#pragma once

#include "jarvis/Export.hpp"
#include "jarvis/Network.hpp"

namespace jar {

class JARC_EXPORT SecureContext {
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