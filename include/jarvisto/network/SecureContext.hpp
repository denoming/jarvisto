#pragma once

#include "jarvisto/network/Export.hpp"
#include "jarvisto/network/Http.hpp"

namespace jar {

class JARVISTO_NETWORK_EXPORT SecureContext {
public:
    SecureContext();

    [[nodiscard]] ssl::context&
    ref();

    [[nodiscard]] const ssl::context&
    ref() const;

private:
    ssl::context _context;
};

} // namespace jar