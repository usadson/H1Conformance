/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "version.hpp"
#include "../../http/response_reader.hpp"
#include "../../http/exception.hpp"

HTTPResponse
Version::Request(const std::string &request) {
    Reconnect();
    connection->Write(request);
    return HTTPResponseReader(connection.get()).Read();
}

/**
 * The HTTP-version is defined as:
 *   HTTP-version  = HTTP-name "/" DIGIT "." DIGIT
 *   HTTP-name     = %x48.54.54.50 ; "HTTP", case-sensitive
 */
void
Version::RunOld() {
    const auto response = Request("GET / HTTP/1.0\r\nHost: " + configuration.hostname + "\r\n\r\n");
    if (response.statusCode <= 100 || response.statusCode >= 400) {
        Failure() << "RunVersionOld: Server reject HTTP/1.0 request with status-code: " << response.statusCode << " (" << response.reasonPhrase << ')';
    }
}

void
Version::RunIncorrectCase() {
    for (const char *version : {"HTTp/1.1", "HTtp/1.1", "Http/1.1", "http/1.1", "hTTP/1.1", "htTP/1.1", "httP/1.1", "htTp/1.1", "hTtp/1.1"}) {
        try {
            std::stringstream request;
            request << "GET / " << version << "\r\nHost: " << configuration.hostname << "\r\n\r\n";
            const HTTPResponse response = Request(request.str());
            if (response.statusCode != 400) {
                if (response.statusCode > 400) {
                    Failure() << "RunVersionIncorrectCase: Server rejected invalid HTTP-version: \"" << version
                              << "\" incorrectly, a 400 (Bad Request) status-code was expected, but got a "
                              << response.statusCode << " (" << response.reasonPhrase << ')';
                } else {
                    Failure() << "RunVersionIncorrectCase: Server accepted malformed HTTP-version: \"" << version
                              << "\", a 400 (Bad Request) status-code was expected, but got a "
                              << response.statusCode << " (" << response.reasonPhrase << ')';
                }
            }
        } catch (const HTTPException &exception) {
            if (exception.Message() =="HTTP Version not in format of \"HTTP/?.?\", was \"<html>\r\n\"") {
                Warning() << "RunLowerCase: Server tried to handle HTTP/1.1 request as an HTTP/0.9 request, when supplied version was: \"" + std::string(version) + "\"";
                continue;
            }
            Warning() << "RunLowerCase: Server sent an invalid HTTP/1.x response. This is probably because the server tried to handle the request as a HTTP/0.9 request."
                      << "Response exception information: "
                      << "\n\tTag: " << exception.Tag()
                      << "\n\tMessage: " << exception.Message()
                      << "\n\tSpecification: " << exception.Specification()
                      << "\n\tSpecification URL: " << exception.URL()
                      << '\n';
            return;
        }
    }
}

void
Version::RunHigherMinor() {
    const auto response = Request("GET / HTTP/1.2\r\nHost: " + configuration.hostname + "\r\n\r\n");
    if (response.statusCode >= 400) {
        Failure() << "RunVersionOld: Server rejected HTTP/1.2 (higher minor) with status-code: " << response.statusCode << " (" << response.reasonPhrase
                  << "). The server should've accepted the request, because the minor is insignificant in terms of parsing and core semantics.";
    }
}

void
Version::RunHigherMajor() {
    const auto response = Request("GET / HTTP/5.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    if (response.statusCode != 505) {
        Failure() << "RunVersionOld: Server accepted HTTP/5.1 (higher major) with status-code: " << response.statusCode << " (" << response.reasonPhrase
                  << "). The server should've rejected the request with status-code 505 (HTTP Version Not Supported) because the major is significant and specifies the syntax & parsing of the message.";
    }
}

void
Version::Run() {
    RunOld();
    RunIncorrectCase();
    RunHigherMinor();
    RunHigherMajor();
}
