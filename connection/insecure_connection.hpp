/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "connection.hpp"

class InsecureConnection : public Connection {
public:
    explicit
    InsecureConnection(const char *address, std::uint16_t port=80);

    ~InsecureConnection() noexcept override;

    char
    ReadChar() override;

    void
    Read(char *buffer, std::size_t count) override;

    void
    Write(const char *buffer, std::size_t count) override;

private:
    int sockfd{};
};
