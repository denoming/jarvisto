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

#include "jarvisto/network/Export.hpp"

#include <boost/asio/cancellation_signal.hpp>

#include <atomic>

namespace io = boost::asio;

namespace jar {

class JARVISTO_NETWORK_EXPORT Cancellable {
public:
    Cancellable() = default;

    virtual ~Cancellable() = default;

    [[nodiscard]] bool
    cancelled() const;

    void
    cancel();

protected:
    [[nodiscard]] io::cancellation_slot
    onCancel();

private:
    std::atomic<bool> _cancelled{false};
    io::cancellation_signal _cancelSig;
};

} // namespace jar