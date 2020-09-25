/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "request_target.hpp"
#include "../../http/response_reader.hpp"

HTTPResponse
RequestTarget::Request(const std::string &request) {
    Reconnect();
    connection->Write(request);
    return HTTPResponseReader(connection.get()).Read();
}

void
RequestTarget::Run() {

}
