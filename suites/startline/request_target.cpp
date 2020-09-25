/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "request_target.hpp"
#include "../../http/response_reader.hpp"

HTTPResponse
RequestTarget::Request(const std::string &requestTarget) {
    Reconnect();
    connection->Write("GET " + requestTarget + " HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    return HTTPResponseReader(connection.get()).Read();
}

void
RequestTarget::RunValidAbsolutePath() {
    const auto absoluteResponse = Request("http://" + configuration.hostname + "/");
    const auto originResponse = Request("/");

    if (absoluteResponse.statusCode != originResponse.statusCode) {
        Failure() << "ValidAbsolutePath: server doesn't recognize absolute-path as a request-target. Status-code: "
                  << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                  << "Read more in RFC 7230 Section 5.3(.2)";
    }
}

void
RequestTarget::Run() {
    RunValidAbsolutePath();
}
