/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../connection/connection.hpp"
#include "response.hpp"

class HTTPResponseReader {
public:
    inline
    HTTPResponseReader(Connection *connection, bool strict=true) noexcept
        : connection(connection), strict(strict) {
    }

    HTTPResponse
    Read();

private:
    Connection *connection;
    bool strict;
    HTTPResponse response{};

    void
    ReadVersion();

    void
    ReadStatusCode();

    void
    ReadReasonPhrase();

};
