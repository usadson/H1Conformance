/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <random>
#include "invalid_request_line.hpp"
#include "../../http/response_reader.hpp"

std::uint16_t
InvalidRequestLine::Request(const std::string &request) {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    return response.statusCode;
}

void
InvalidRequestLine::RunMethodValid() {
    const std::string suffix = " / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    for (std::size_t i = 0; i < 15; i++) {
        const auto method = configuration.utils.GenerateRandomLengthToken();
        const auto statusCode = Request(method + suffix);

        if (statusCode >= 400) {
            auto message = Failure()
                    << "Server denied valid method: \"" << method
                    << "\" with status-code: " << statusCode
                    << ". A method is defined as a token by RFC 7230 Section 3.1.1. A token is defined as 1*tchar by RFC 7230 Section 3.2.6. A tchar is defined as a VCHAR without delimiters.";
        }

        Reconnect();
    }
}

void
InvalidRequestLine::Run() {
    RunMethodValid();
}
