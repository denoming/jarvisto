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