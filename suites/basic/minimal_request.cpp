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
        if ((response.statusCode > 208 && response.statusCode < 300) || (response.statusCode > 309 && response.statusCode < 400) || response.statusCode == 306) {
            Warning() << "Warning: unregistered status-code: " << response.statusCode
                      << " with reason-phrase: \"" << response.reasonPhrase << "\"."
                      "Status codes should (generally) be registered with IANA, although this is not required as per RFC 7231 Section 8.2.2. "
                      "All registered status codes can be found at https://www.iana.org/assignments/http-status-codes/http-status-codes.xhtml";
        }
    } else if (response.statusCode >= 600) {
        // TODO
    } else {
        std::exception();
    }
}
