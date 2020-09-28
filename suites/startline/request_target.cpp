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
RequestTarget::RunInvalidAbsolutePath() {
    // Note: The query isn't required to have the syntax of application/x-www-form-urlencoded
    for (const auto &requestTarget : {"httpinvalid://" + configuration.hostname, "ssh://" + configuration.hostname}) {
        const auto absoluteResponse = Request(requestTarget);

        if (absoluteResponse.statusCode != 400) {
            Failure() << "InvalidAbsolutePath: server accepted invalid absolute-form: \"" << requestTarget << "\" as request-target. Status-code: "
                      << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                      << "Read more in RFC 7230 Section 5.3(.4)";
        }
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
RequestTarget::RunValidAbsoluteWithQuery() {
    const std::string prefix = "http://" + configuration.hostname;
    // Note: The query isn't required to have the syntax of application/x-www-form-urlencoded
    for (const auto *requestTarget : {"/?a=b", "/?a=b&b=c", "/?????", "/?/test"}) {
        const auto absoluteResponse = Request(prefix + requestTarget);

        if (absoluteResponse.statusCode >= 400) {
            Failure() << "ValidOriginWithQuery: server rejected absolute-form with query: \"" << prefix << requestTarget
                      << "\" as request-target. Status-code: "
                      << absoluteResponse.statusCode << " (" << absoluteResponse.reasonPhrase << ").\n"
                      << "Read more in RFC 7230 Section 5.3(.3)";
        }
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

#define ADD_FUNC(i, s) { \
    names[i] = #s; \
    functions[i] = &RequestTarget::s; \
}

void
RequestTarget::Run() {
    constexpr std::size_t size = 6;

    std::array<void (RequestTarget::*)(), size> functions;
    std::array<const char *, size> names;

    ADD_FUNC(0, RunAsteriskInNonOptions);
    ADD_FUNC(1, RunAuthorityInNonConnect);
    ADD_FUNC(2, RunInvalidAbsolutePath);
    ADD_FUNC(3, RunValidAbsolutePath);
    ADD_FUNC(4, RunValidAbsoluteWithQuery);
    ADD_FUNC(5, RunValidOriginWithQuery);

    for (std::size_t i = 0; i < size; i++) {
        PushSection(names[i]);
        (this->*functions[i])();
        PopSection();
    }
}
