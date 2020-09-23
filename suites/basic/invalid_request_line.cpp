/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "invalid_request_line.hpp"
#include "../../http/response_reader.hpp"

std::uint16_t
InvalidRequestLine::Request(const std::string &request) {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    return response.statusCode;
}

void
InvalidRequestLine::Run() {

}
