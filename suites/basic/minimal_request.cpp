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
    const HTTPResponse response = reader.Read();

    if (response.statusCode < 400) {
        if ((response.statusCode > 208 && response.statusCode < 300) || (response.statusCode > 309 && response.statusCode < 400) || response.statusCode == 306 || response.statusCode >= 600) {
            std::cerr << "Warning: unregistered status-code: " << response.statusCode << " with reason-phrase: \"" << response.reasonPhrase << "\"\n";
        }
    } else {
        throw std::exception();
    }

    Warning() << "Tset!";
}
