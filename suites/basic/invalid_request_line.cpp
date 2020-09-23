/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <random>
#include "invalid_request_line.hpp"
#include "../../http/response_reader.hpp"

std::uint16_t
InvalidRequestLine::Request(const std::string &request) {
    connection->Write("GET / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n");
    const HTTPResponse response = HTTPResponseReader(connection.get()).Read();
    return response.statusCode;
}

void
InvalidRequestLine::RunMethod() {
    std::random_device device;
    std::default_random_engine engine(device());
    std::uniform_int_distribution<int> distribution(0x20, 0x7E);

    const auto randomTokenCharacterGenerator = [&distribution, &engine]() mutable -> char {
        static const char disallowedCharacters[] = "\"(),/:;<=>?@[\\]{}";
        while (true) {
            char c = distribution(engine);
            if (std::find(std::cbegin(disallowedCharacters), std::cend(disallowedCharacters), c) == std::cend(disallowedCharacters)) {
                return c;
            }
        }
    };

    const std::string suffix = " / HTTP/1.1\r\nHost: " + configuration.hostname + "\r\n\r\n";
    std::vector<char> buffer;
    for (std::size_t i = 0; i < 15; i++) {
        buffer.resize(std::uniform_int_distribution<int>(0, 15)(engine));
        std::generate(std::begin(buffer), std::end(buffer), randomTokenCharacterGenerator);

        const auto statusCode = Request(std::string(std::cbegin(buffer), std::cend(buffer)) + suffix);
        std::cout << std::string(std::cbegin(buffer), std::cend(buffer)) << '\n';
        if (statusCode >= 400) {
            auto message = Failure() << "Server denied valid method: \"";
            message.Stream()->write(buffer.data(), buffer.size());
            message << "\" with status-code: " << statusCode << ". A method is defined as a token by RFC 7230 Section 3.1.1. A token is defined as 1*tchar by RFC 7230 Section 3.2.6. A tchar is defined as a VCHAR without delimiters.";
        }

        Reconnect();
    }
}

void
InvalidRequestLine::Run() {
    RunMethod();
}
