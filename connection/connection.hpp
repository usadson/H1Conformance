/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <cstdint>

#include <vector>

class Connection {
public:
    inline
    Connection(const char *address, std::uint16_t port) noexcept
        : address(address), port(port) {
    }

    virtual
    ~Connection() = default;

    [[nodiscard]] virtual char
    ReadChar() = 0;

    virtual void
    Read(std::vector<char>::iterator iter, std::size_t amount) = 0;

    [[nodiscard]] inline constexpr const char *
    Address() const noexcept {
        return address;
    }

    [[nodiscard]] inline constexpr std::uint16_t
    Port() const noexcept {
        return port;
    }

protected:
    const char *address;
    std::uint16_t port;
};
