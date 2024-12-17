#include "jarvisto/network/Cancellable.hpp"

namespace jar {

bool
Cancellable::cancelled() const
{
    return _cancelled;
}

void
Cancellable::cancel()
{
    _cancelled = true;

    _cancelSig.emit(io::cancellation_type::terminal);
}

io::cancellation_slot
Cancellable::onCancel()
{
    return _cancelSig.slot();
}

} // namespace jar