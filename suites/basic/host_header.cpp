/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "host_header.hpp"

#include "../../http/response_reader.hpp"

void
HostHeader::RunWithout() {
    connection->Write("GET / HTTP/1.1\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode != 400) {
        Failure() << "RunWithout: no 'Host' was sent and the server accepted it with status-code: " << response.statusCode << ". A Host header is required as per RFC 7230 Section 5.4. and if it isn't present the server must respond with status-code 404 (Bad Request)";
    }
}

void
HostHeader::RunWith() {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode <= 0 || response.statusCode >= 400) {
        Failure() << "RunWith: Failed with status code " << response.statusCode << " and reason phrase \"" << response.reasonPhrase << '"';
    }
}

void
HostHeader::RunWithMultiple() {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    if (response.statusCode != 400) {
        Failure() << "RunWithout: multiple 'Host' headers were sent and the server accepted it with status-code: " << response.statusCode << ". Exactly one Host header is required as per RFC 7230 Section 5.4. and if it isn't present, or there are more than one, the server must respond with status-code 404 (Bad Request)";
    }
}

void
HostHeader::Run() {
    RunWith();
    Reconnect();
    RunWithout();
    Reconnect();
    RunWithMultiple();
}
