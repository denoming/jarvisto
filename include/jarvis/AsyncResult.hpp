#pragma once

#include <boost/system/error_code.hpp>

#include <concepts>
#include <functional>
#include <future>
#include <memory>
#include <variant>

namespace sys = boost::system;

namespace jar {

namespace details {

template<typename T>
inline bool
ready(const std::shared_future<T>& future)
{
    return (future.wait_for(std::chrono::seconds{0}) == std::future_status::ready);
}

} // namespace details

template<typename T>
class AsyncResultSetter;

template<typename T>
class AsyncResult final {
public:
    using Setter = AsyncResultSetter<T>;

    AsyncResult() = default;

    AsyncResult(AsyncResult&&) noexcept = default;
    AsyncResult&
    operator=(AsyncResult&&) noexcept
        = default;

    AsyncResult(const AsyncResult&) = delete;
    AsyncResult&
    operator=(const AsyncResult&)
        = delete;

    [[nodiscard]] bool
    valid() const
    {
        return _future.valid();
    }

    [[nodiscard]] bool
    ready() const
    {
        return details::ready(_future);
    }

    void
    wait()
    {
        _future.wait();
    }

    [[nodiscard]] const T&
    value() const
    {
        return std::get<1>(_future.get());
    }

    [[nodiscard]] std::error_code
    error() const
    {
        return std::get<0>(_future.get());
    }

private:
    using Result = std::variant<std::error_code, T>;

    template<typename U>
    friend class AsyncResultSetter;

    explicit AsyncResult(std::shared_future<Result> future)
        : _future{std::move(future)}
    {
    }

private:
    std::shared_future<Result> _future;
};

template<typename T>
class AsyncResultSetter {
public:
    using OnReady = void(const T& value);
    using OnError = void(std::error_code error);

    AsyncResultSetter()
        : _future{_result.get_future()}
    {
    }

    template<std::invocable<const T&> C1, std::invocable<std::error_code> C2>
    AsyncResultSetter(C1&& readyCallback, C2&& errorCallback)
        : _future{_result.get_future()}
        , _onReady{std::forward<C1>(readyCallback)}
        , _onError{std::forward<C2>(errorCallback)}
    {
    }

    [[nodiscard]] bool
    dirty() const
    {
        return details::ready(_future);
    }

    void
    setValue(const T& input)
    {
        if (dirty()) {
            throw std::logic_error{"Already has value"};
        }

        _result.set_value(Result{std::in_place_index<1>, input});

        if (_onReady) {
            auto r = getResult();
            _onReady(r.value());
        }
    }

    void
    setValue(T&& input)
    {
        if (dirty()) {
            throw std::logic_error{"Already has value"};
        }

        _result.set_value(Result{std::in_place_index<1>, std::move(input)});

        if (_onReady) {
            auto r = getResult();
            _onReady(r.value());
        }
    }

    void
    setError(std::error_code input)
    {
        if (dirty()) {
            throw std::logic_error{"Already has value"};
        }

        _result.set_value(Result{std::in_place_index<0>, input});

        if (_onError) {
            auto r = getResult();
            _onError(r.error());
        }
    }

    template<typename... Args>
    void
    emplaceValue(Args&&... args)
    {
        if (dirty()) {
            throw std::logic_error{"Already has value"};
        }

        _result.set_value(Result{std::in_place_index<1>, std::forward<Args>(args)...});

        if (_onReady) {
            auto r = getResult();
            _onReady(r.value());
        }
    }

    AsyncResult<T>
    getResult()
    {
        return AsyncResult<T>{_future};
    }

private:
    using Result = std::variant<std::error_code, T>;
    std::function<OnReady> _onReady;
    std::function<OnError> _onError;
    std::promise<Result> _result;
    std::shared_future<Result> _future;
};

template<typename T>
auto
makeResultSetter()
{
    return AsyncResultSetter<T>();
}

template<typename T>
auto
makeResultSetter(std::invocable<const T&> auto&& onReady,
                 std::invocable<std::error_code> auto&& onError)
{
    return AsyncResultSetter<T>(std::forward<decltype(onReady)>(onReady),
                                std::forward<decltype(onError)>(onError));
}

} // namespace jar