/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "minimal_request.hpp"

#include <iostream>

void
MinimalRequest::Run() {
    std::string request = "GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    std::cout << "Hello!\n";

    connection->Write(request);
    while (true) {
        std::cout << connection->ReadChar() << std::flush;
    }
}
