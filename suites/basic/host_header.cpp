/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "host_header.hpp"

#include "../../http/response_reader.hpp"

/* This suite checks the requirements of RFC 7230 Section 5.4.
 * Read more at: https://tools.ietf.org/html/rfc7230#section-5.4 */

/**
 * Servers receiving a HTTP/1.1 request without a 'Host' header should reject
 * the request with a 400 (Bad Request) status-code response.
 */
void
HostHeader::RunWithout() {
    connection->Write("GET / HTTP/1.1\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode != 400) {
        Failure() << "RunWithout: no 'Host' was sent and the server accepted it with status-code: " << response.statusCode << ". A Host header is required as per RFC 7230 Section 5.4. and if it isn't present the server must respond with status-code 404 (Bad Request)";
    }
}

/**
 * A HTTP/1.1 GET request with a Host header is the minimal required metadata
 * in a request, thus is expected to be accepted.
 */
void
HostHeader::RunWith() {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode <= 100 || response.statusCode >= 400) {
        Failure() << "RunWith: Failed with status code " << response.statusCode << " and reason phrase \"" << response.reasonPhrase << '"';
    }
}

/**
 * A HTTP/1.1 request with multiple Host headers should be rejected with a
 * 400 (Bad Request) status code.
 */
void
HostHeader::RunWithMultiple() {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    if (response.statusCode != 400) {
        Failure() << "RunWithout: multiple 'Host' headers were sent and the server accepted it with status-code: " << response.statusCode << ". Exactly one Host header is required as per RFC 7230 Section 5.4. and if it isn't present, or there are more than one, the server must respond with status-code 404 (Bad Request)";
    }
}

/**
 * Make a HTTP/1.0 request without the 'Host' header.
 *
 * Since HTTP/1.1 the header is required, but in HTTP/1.0 not. Servers must
 * deny HTTP/1.1 requests without a 'Host' header, and only HTTP/1.1 requests.
 *
 * Excerpt from RFC 7230 Section 5.4.:
 * "A server MUST respond with a 400 (Bad Request) status code to any
 * HTTP/1.1 request message that lacks a Host header field and to any
 * request message that contains more than one Host header field or a
 * Host header field with an invalid field-value."
 */
void
HostHeader::RunWithoutHTTP10() {
    connection->Write("GET / HTTP/1.0\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode <= 100 || response.statusCode >= 400) {
        Failure() << "Server rejected HTTP/1.0 request without (optional in HTTP/1.0) Host header. The server must only reject HTTP/1.1 requests without a Host header. The status-code was: " << response.statusCode << " and reason-phrase: \"" << response.reasonPhrase << "\"";
    }
}

/**
 * Make a HTTP/1.1 request with the 'Host' header, but include the port number.
 *
 * The port number is optional, but is allowed.
 *
 * Excerpt from RFC 7230 Section 5.4.:
 * "The "Host" header field in a request provides the host and port
 * information from the target URI, enabling the origin server to
 * distinguish among resources while servicing requests for multiple
 * host names on a single IP address.
 *
 *  Host = uri-host [ ":" port ] ; Section 2.7.1"
 */
void
HostHeader::RunWithPort() {
    std::stringstream request;
    request << "GET / HTTP/1.1\r\nHost: " << configuration.hostname << ':' << configuration.port << "\r\n\r\n";
    connection->Write(request.str());
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode <= 100 || response.statusCode >= 400) {
        Failure() << "RunWithPort: UA may include a port number with the hostname, but the server rejected the request with status-code " << response.statusCode << " (" << response.reasonPhrase << ')';
    }
}

/**
 * The server must also reject the request if the 'Host' header is inaccurate
 * (i.e. not (one of) the domain name(s) of the server).
 *
 * The value is www.test.invalid, and isn't assignable as per RFC 2606, so is
 * future-proof.
 *
 * Excerpt from RFC 7230 Section 5.4.:
 * "A server MUST respond with a 400 (Bad Request) status code to any
 * HTTP/1.1 request message ... or a Host header field with an invalid
 * field-value."
 */
void
HostHeader::RunWithInaccurateValue() {
    connection->Write("GET / HTTP/1.1\r\nHost: www.test.invalid\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    if (response.statusCode != 400) {
        Failure() << "RunWithInaccurateValue: an invalid Host header was accepted by the server with status-code " << response.statusCode << " (" << response.reasonPhrase << "). When the server receives a Host header with an invalid field-value, the server must respond with status-code 404 (Bad Request)";
    }
}

void
HostHeader::RunWithIllegalValue() {
    for (const auto &string : {".test", "local..host"}) {
        connection->Write("GET / HTTP/1.1\r\nHost: " + std::string(string) + "\r\n\r\n");
        const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
        if (response.statusCode != 400) {
            Failure() << "RunWithIllegalValue: an illegal Host header was accepted by the server with status-code " << response.statusCode << " (" << response.reasonPhrase << "). When the server receives a Host header with an invalid field-value, the server must respond with status-code 404 (Bad Request)";
        }
        Reconnect();
    }
}

void
HostHeader::Run() {
    RunWith();
    Reconnect();
    RunWithout();
    Reconnect();
    RunWithMultiple();
    Reconnect();
    RunWithoutHTTP10();
    Reconnect();
    RunWithPort();
    Reconnect();
    RunWithInaccurateValue();
    Reconnect();
    RunWithIllegalValue();
}
