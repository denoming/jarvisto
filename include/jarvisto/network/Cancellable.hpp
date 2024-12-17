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