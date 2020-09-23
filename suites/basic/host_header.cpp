/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "host_header.hpp"

#include "../../http/response_reader.hpp"

void
HostHeader::RunWithout() {

}

void
HostHeader::RunWith() {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();

    if (response.statusCode >= 400) {
        Failure() << "RunWith: Failed with status code " << response.statusCode << " and reason phrase \"" << response.reasonPhrase << '"';
    }
}

void
HostHeader::RunWithMultiple() {

}

void
HostHeader::Run() {
    RunWith();
    Reconnect();
    RunWithout();
    Reconnect();
    RunWithMultiple();
}
