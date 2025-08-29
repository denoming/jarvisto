// Copyright 2025 Denys Asauliak
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "jarvisto/network/Mqtt.hpp"
#include "jarvisto/network/MqttClient.hpp"
#include "jarvisto/network/Asio.hpp"

#include <sigc++/adaptors/track_obj.h>

#include <memory>
#include <mutex>
#include <map>

namespace jar {

namespace detail {

class MqttHandler {
public:
    using Ptr = std::unique_ptr<MqttHandler>;

    template<typename T, typename... Args>
    void
    call(Args&&... args)
    {
        if (T* typedSelf = dynamic_cast<T*>(this); typedSelf != nullptr) {
            (*typedSelf)(std::forward<Args>(args)...);
        }
    }

    virtual ~MqttHandler() = default;
};

class MqttConnDiscHandler final : public MqttHandler {
public:
    explicit MqttConnDiscHandler(
        io::any_io_executor executor,
        io::completion_handler_for<void(std::error_code, MqttReturnCode)> auto handler)
        : _executor{std::move(executor)}
        , _handler{std::move(handler)}
    {
    }

    void
    operator()(std::error_code ec, MqttReturnCode rc)
    {
        io::post(_executor, [ec, rc, handler = std::move(_handler)]() mutable { handler(ec, rc); });
    }

private:
    io::any_io_executor _executor;
    io::any_completion_handler<void(std::error_code, MqttReturnCode)> _handler;
};

class MqttCompletionHandler final : public MqttHandler {
public:
    explicit MqttCompletionHandler(io::any_io_executor executor,
                                   io::completion_handler_for<void(std::error_code)> auto handler)
        : _executor{std::move(executor)}
        , _handler{std::move(handler)}
    {
    }

    void
    operator()(std::error_code ec)
    {
        io::post(_executor, [ec, handler = std::move(_handler)]() mutable { handler(ec); });
    }

private:
    io::any_io_executor _executor;
    io::any_completion_handler<void(std::error_code)> _handler;
};

} // namespace detail

class MqttAsyncClient : public sigc::trackable {
public:
    /* Signatures */
    using OnMessage = void(int mid, std::string_view topic, void* payload, size_t size);
    using OnLog = void(int logLevel, std::string_view message);
    /* Signals */
    using OnMessageSignal = sigc::signal<OnMessage>;
    using OnLogSignal = sigc::signal<OnLog>;

    explicit MqttAsyncClient(io::any_io_executor executor,
                             const char* id = nullptr,
                             bool cleanSession = true,
                             bool getLogs = false)
        : _client{id, cleanSession, getLogs}
        , _executor{std::move(executor)}
    {
        std::ignore = _client.onConnect().connect(
            sigc::track_obj(std::bind_front(&MqttAsyncClient::onConnect, this), this));
        std::ignore = _client.onDisconnect().connect(
            sigc::track_obj(std::bind_front(&MqttAsyncClient::onDisconnect, this), this));
        std::ignore = _client.onSubscribe().connect(
            sigc::track_obj(std::bind_front(&MqttAsyncClient::onSubscribe, this), this));
        std::ignore = _client.onUnsubscribe().connect(
            sigc::track_obj(std::bind_front(&MqttAsyncClient::onUnsubscribe, this), this));
        std::ignore = _client.onPublish().connect(
            sigc::track_obj(std::bind_front(&MqttAsyncClient::onPublish, this), this));
    }

    [[nodiscard]] bool
    hasConnection() const
    {
        return _client.hasConnection();
    }

    [[nodiscard]] std::error_code
    credentials(const std::string_view user, const std::string_view password) const
    {
        return _client.credentials(user, password);
    }

    template<io::completion_token_for<void(std::error_code, MqttReturnCode)> CompletionToken>
    auto
    connect(std::string_view host, uint16_t port, int keepAlive, CompletionToken&& token)
    {
        auto initiate =
            [this](io::completion_handler_for<void(std::error_code, MqttReturnCode)> auto&& handler,
                   std::string_view host,
                   uint16_t port,
                   int keepAlive) {
                io::any_io_executor executor = io::get_associated_executor(handler, _executor);
                if (const auto ec = _client.connectAsync(host, port, keepAlive); ec) {
                    io::post(executor,
                             [ec, handler = std::forward<decltype(handler)>(handler)]() mutable {
                                 handler(ec, MqttReturnCode::Unknown);
                             });
                } else {
                    pushHandler<detail::MqttConnDiscHandler>(
                        kConnHandlerId,
                        std::move(executor),
                        std::forward<decltype(handler)>(handler));
                }
            };

        return io::async_initiate<CompletionToken, void(std::error_code, MqttReturnCode)>(
            initiate, token, host, port, keepAlive);
    }

    template<io::completion_token_for<void(std::error_code, MqttReturnCode)> CompletionToken>
    auto
    disconnect(CompletionToken&& token)
    {
        auto initiate =
            [this](
                io::completion_handler_for<void(std::error_code, MqttReturnCode)> auto&& handler) {
                io::any_io_executor executor = io::get_associated_executor(handler, _executor);
                if (const auto ec = _client.disconnect(); ec) {
                    io::post(executor,
                             [ec, handler = std::forward<decltype(handler)>(handler)]() mutable {
                                 handler(ec, MqttReturnCode::Unknown);
                             });
                } else {
                    pushHandler<detail::MqttConnDiscHandler>(
                        kDiscHandlerId,
                        std::move(executor),
                        std::forward<decltype(handler)>(handler));
                }
            };

        return io::async_initiate<CompletionToken, void(std::error_code, MqttReturnCode)>(initiate,
                                                                                          token);
    }

    template<io::completion_token_for<void(std::error_code)> CompletionToken>
    auto
    subscribe(std::string_view topic, MqttQoS qos, CompletionToken&& token)
    {
        auto initiate = [this](io::completion_handler_for<void(std::error_code)> auto&& handler,
                               std::string_view topic,
                               MqttQoS qos) {
            io::any_io_executor executor = io::get_associated_executor(handler, _executor);
            if (const auto result = _client.subscribe(topic, qos); not result) {
                io::post(executor,
                         [ec = result.error(),
                          handler = std::forward<decltype(handler)>(handler)]() mutable {
                             handler(ec);
                         });
            } else {
                pushHandler<detail::MqttCompletionHandler>(
                    result.value(), std::move(executor), std::forward<decltype(handler)>(handler));
            }
        };

        return io::async_initiate<CompletionToken, void(std::error_code)>(
            initiate, token, topic, qos);
    }

    template<io::completion_token_for<void(std::error_code)> CompletionToken>
    auto
    unsubscribe(std::string_view topic, CompletionToken&& token)
    {
        auto initiate = [this](io::completion_handler_for<void(std::error_code)> auto&& handler,
                               std::string_view topic) {
            io::any_io_executor executor = io::get_associated_executor(handler, _executor);
            if (const auto result = _client.unsubscribe(topic); not result) {
                io::post(executor,
                         [ec = result.error(),
                          handler = std::forward<decltype(handler)>(handler)]() mutable {
                             handler(ec);
                         });
            } else {
                pushHandler<detail::MqttCompletionHandler>(
                    result.value(), std::move(executor), std::forward<decltype(handler)>(handler));
            }
        };

        return io::async_initiate<CompletionToken, void(std::error_code)>(initiate, token, topic);
    }

    template<io::completion_token_for<void(std::error_code)> CompletionToken>
    auto
    publish(std::string_view topic,
            const void* payload,
            std::size_t size,
            MqttQoS qos,
            bool retain,
            CompletionToken&& token)
    {
        auto initiate = [this](io::completion_handler_for<void(std::error_code)> auto&& handler,
                               std::string_view topic,
                               io::const_buffer buffer,
                               MqttQoS qos,
                               bool retain) {
            io::any_io_executor executor = io::get_associated_executor(handler, _executor);
            const auto result = _client.publish(topic, buffer.data(), buffer.size(), qos, retain);
            if (not result) {
                io::post(executor,
                         [ec = result.error(),
                          handler = std::forward<decltype(handler)>(handler)]() mutable {
                             handler(ec);
                         });
            } else {
                pushHandler<detail::MqttCompletionHandler>(
                    result.value(), std::move(executor), std::forward<decltype(handler)>(handler));
            }
        };

        return io::async_initiate<CompletionToken, void(std::error_code)>(
            initiate, token, topic, io::buffer(payload, size), qos, retain);
    }

    template<io::completion_token_for<void(std::error_code)> CompletionToken>
    auto
    publish(std::string_view topic,
            std::string_view payload,
            MqttQoS qos,
            bool retain,
            CompletionToken&& token)
    {
        return publish(topic,
                       payload.data(),
                       payload.size(),
                       qos,
                       retain,
                       std::forward<CompletionToken>(token));
    }

    [[maybe_unused]] OnMessageSignal
    onMessage() const
    {
        return _client.onMessage();
    }

    [[maybe_unused]] OnLogSignal
    onLog() const
    {
        return _client.onLog();
    }

private:
    using HandlerId = int32_t;

    template<typename HandlerT, typename... Args>
    void
    callHandler(const HandlerId id, Args&&... args)
    {
        std::unique_lock lock{_guard};
        if (const auto hit = _handlers.find(id); hit != std::end(_handlers)) {
            detail::MqttHandler::Ptr handler{std::move(std::get<detail::MqttHandler::Ptr>(*hit))};
            std::ignore = _handlers.erase(hit);
            lock.unlock();
            handler->call<HandlerT>(std::forward<Args>(args)...);
        }
    }

    template<typename HandlerT>
    void
    pushHandler(const HandlerId id, io::any_io_executor executor, auto&& handler)
    {
        std::lock_guard lock{_guard};
        assert(not _handlers.contains(id));
        std::ignore = _handlers.insert(
            {id,
             std::make_unique<HandlerT>(std::move(executor),
                                        std::forward<decltype(handler)>(handler))});
    }

    void
    onConnect(const MqttReturnCode rc)
    {
        callHandler<detail::MqttConnDiscHandler>(kConnHandlerId, MqttErrorCode::Success, rc);
    }

    void
    onDisconnect(const MqttReturnCode rc)
    {
        callHandler<detail::MqttConnDiscHandler>(kDiscHandlerId, MqttErrorCode::Success, rc);
    }

    void
    onSubscribe(const int mid)
    {
        callHandler<detail::MqttCompletionHandler>(mid, MqttErrorCode::Success);
    }

    void
    onUnsubscribe(const int mid)
    {
        callHandler<detail::MqttCompletionHandler>(mid, MqttErrorCode::Success);
    }

    void
    onPublish(const int mid)
    {
        callHandler<detail::MqttCompletionHandler>(mid, MqttErrorCode::Success);
    }

private:
    static constexpr HandlerId kConnHandlerId{std::numeric_limits<HandlerId>::min() + 1};
    static constexpr HandlerId kDiscHandlerId{std::numeric_limits<HandlerId>::min() + 2};

    io::any_io_executor _executor;
    MqttClient _client;

    mutable std::mutex _guard;
    std::map<HandlerId, detail::MqttHandler::Ptr> _handlers;
};

} // namespace jar