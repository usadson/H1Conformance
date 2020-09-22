/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "minimal_request.hpp"

#include <iostream>

#include "../../http/response_reader.hpp"

void
MinimalRequest::Run() {
    std::string request = "GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    connection->Write(request);

    HTTPResponseReader reader(connection.get());
    reader.Read();
}
