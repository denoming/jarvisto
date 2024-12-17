#include "jarvisto/network/SecureContext.hpp"

namespace jar {

SecureContext::SecureContext()
    : _context{ssl::context::tlsv12}
{
    _context.set_default_verify_paths();
    _context.set_verify_mode(ssl::verify_peer);
}

ssl::context&
SecureContext::ref()
{
    return _context;
}

const ssl::context&
SecureContext::ref() const
{
    return _context;
}

} // namespace jar