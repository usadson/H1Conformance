/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <random>
#include "invalid_request_line.hpp"
#include "../../http/response_reader.hpp"

std::uint16_t
InvalidRequestLine::Request(const std::string &request) {
    connection->Write(request);
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    return response.statusCode;
}

void
InvalidRequestLine::RunMethodValid() {
    const std::string suffix = " / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    for (std::size_t i = 0; i < 15; i++) {
        const auto method = configuration.utils.GenerateRandomLengthToken();
        const auto statusCode = Request(method + suffix);

        if (statusCode >= 400 && statusCode < 500) {
            auto message = Failure()
                    << "Server denied valid method: \"" << method
                    << "\" with status-code: " << statusCode
                    << ". A method is defined as a token by RFC 7230 Section 3.1.1. A token is defined as 1*tchar by RFC 7230 Section 3.2.6. A tchar is defined as a VCHAR without delimiters.";
        }

        Reconnect();
    }
}

void
InvalidRequestLine::RunMethodInvalid() {
    const std::string suffix = " / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    std::array illegalCharacters = {
            '\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x07', '\x08',
            '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
            '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x17', '\x18',
            '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
            '"', '(', ')', ',', '/', ':', ';', '<', '=',
            '>', '?', '@', '[', '\\', ']', '{', '}'
    };
    for (std::size_t i = 0; i < 15; i++) {
        auto method = configuration.utils.GenerateRandomLengthToken();

        for (std::size_t i = 0; i < 3; i++) {
            auto iindex = configuration.utils.GenerateRandomNumber<std::size_t>(0, illegalCharacters.size() - 1);
            auto oindex = configuration.utils.GenerateRandomNumber<std::size_t>(0, method.length() - 1);
            method[oindex] = illegalCharacters[iindex];
        }

        const auto statusCode = Request(method + suffix);

        if (statusCode != 400) {
            auto message = Failure()
                    << "Server accepted invalid method: \"" << method
                    << "\" with non-400 status-code: " << statusCode
                    << ". A method is defined as a token by RFC 7230 Section 3.1.1. A token is defined as 1*tchar by RFC 7230 Section 3.2.6. A tchar is defined as a VCHAR without delimiters.";
        }

        Reconnect();
    }
}


/**
 * The HTTP-version is defined as:
 *   HTTP-version  = HTTP-name "/" DIGIT "." DIGIT
 *   HTTP-name     = %x48.54.54.50 ; "HTTP", case-sensitive
 */
void
InvalidRequestLine::RunVersionOld() {
    connection->Write("GET / HTTP/1.0\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    if (response.statusCode <= 100 || response.statusCode >= 400) {
        Failure() << "RunVersionOld: Server reject HTTP/1.0 request with status-code: " << response.statusCode << " (" << response.reasonPhrase << ')';
    }
}

void
InvalidRequestLine::RunVersionIncorrectCase() {
    for (const char *version : {"HTTp/1.1", "HTtp/1.1", "Http/1.1", "http/1.1", "hTTP/1.1", "htTP/1.1", "httP/1.1", "htTp/1.1", "hTtp/1.1"}) {
        std::stringstream request;
        request << "GET / " << version << "\r\nHost: " << configuration.hostname << "\r\n\r\n";
        connection->Write(request.str());
        const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
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
    }
}

void
InvalidRequestLine::Run() {
    RunMethodValid();
    RunMethodInvalid();

    RunVersionOld();
    RunVersionIncorrectCase();
}
