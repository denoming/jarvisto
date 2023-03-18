#pragma once

#include "jarvis/CommonExport.hpp"

#include <boost/asio/bind_cancellation_slot.hpp>
#include <boost/asio/cancellation_signal.hpp>

#include <atomic>

namespace io = boost::asio;

namespace jar {

class JARC_EXPORT Cancellable {
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