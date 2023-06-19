#pragma once

#include "jarvisto/Export.hpp"
#include "jarvisto/Network.hpp"

namespace jar {

class JARVISTO_EXPORT SecureContext {
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