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
RequestTarget::RunAsteriskInNonOptions() {
    const auto absoluteResponse = Request("*");

    if (absoluteResponse.statusCode != 400) {
        Failure() << "AsteriskInNonOptions: server accepted asterisk-form as request-target in a non-OPTIONS request. Status-code: "
                  << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                  << "Read more in RFC 7230 Section 5.3(.4)";
    }
}

void
RequestTarget::RunAuthorityInNonConnect() {
    const auto absoluteResponse = Request(configuration.hostname);

    if (absoluteResponse.statusCode != 400) {
        Failure() << "AuthorityInNonConnect: server accepted authority-form as request-target in a non-CONNECT request. Status-code: "
                  << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                  << "Read more in RFC 7230 Section 5.3(.3)";
    }
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
RequestTarget::RunValidOriginWithQuery() {
    // Note: The query isn't required to have the syntax of application/x-www-form-urlencoded
    for (const auto *requestTarget : {"/?a=b", "/?a=b&b=c", "/?????", "/?/test"}) {
        const auto absoluteResponse = Request(requestTarget);

        if (absoluteResponse.statusCode >= 400) {
            Failure() << "ValidOriginWithQuery: server rejected origin with query: \"" << requestTarget << "\" as request-target. Status-code: "
                      << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                      << "Read more in RFC 7230 Section 5.3(.3)";
        }
    }
}

void
RequestTarget::Run() {
    RunAsteriskInNonOptions();
    RunAuthorityInNonConnect();
    RunValidAbsolutePath();
    RunValidOriginWithQuery();
}
