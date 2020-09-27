/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "response_reader.hpp"

#include <array>
#include <vector>
#include <sstream>

#include "exception.hpp"

HTTPResponse
HTTPResponseReader::Read() {
    ReadVersion();
    ReadStatusCode();
    ReadReasonPhrase();
    return response;
}

void
HTTPResponseReader::ReadVersion() {
    if (strict) {
        std::array<char, 9> buffer{};
        connection->Read(buffer);
        response.version = std::string(std::cbegin(buffer), std::cbegin(buffer) + 8);

        if (buffer[0] != 'H' || buffer[1] != 'T' || buffer[2] != 'T' || buffer[3] != 'P' || buffer[4] != '/' || buffer[6] != '.') {
            throw HTTPException(__PRETTY_FUNCTION__ , "HTTP Version not in format of \"HTTP/?.?\", was \"" + response.version + '"',
                                "RFC 7230 Section 2.6",
                                "https://tools.ietf.org/html/rfc7230#section-2.6");
        }

        if (buffer[5] != '1' || (buffer[7] != '0' && buffer[7] != '1')) {
            throw HTTPException(__PRETTY_FUNCTION__ , "Invalid HTTP version major or minor. Should be one of \"HTTP/1.0\" or \"HTTP/1.1\", not \"" + response.version + '"',
                                "RFC 7230 Section 2.6",
                                "https://tools.ietf.org/html/rfc7230#section-2.6");
        }

        if (buffer[8] != ' ') {
            throw HTTPException(__PRETTY_FUNCTION__ , std::string("A HTTP version should be followed by 0x20 SPACE, not a '") + buffer[8] + "'.",
                                "RFC 7230 Section 3.1.2",
                                "https://tools.ietf.org/html/rfc7230#section-3.1.2");
        }
    } else {
        std::vector<char> buffer;
        char c;
        while ((c = connection->ReadChar()) != ' ') {
            buffer.push_back(c);
        }
        response.version = std::string(std::begin(buffer), std::end(buffer));
    }
}

constexpr bool
EnsureCharIsDigit(char c) {
    return c >= '0' && c <= '9';
}

constexpr std::uint8_t
ConvertCharToSingleDigit(char c) noexcept {
    return static_cast<std::uint8_t>(c - '0');
}

void
HTTPResponseReader::ReadStatusCode() {
    std::array<char, 3> buffer;
    connection->Read(buffer);
    if (!EnsureCharIsDigit(buffer[0]) || !EnsureCharIsDigit(buffer[1]) || !EnsureCharIsDigit(buffer[2])) {
        throw HTTPException(__PRETTY_FUNCTION__ , std::string("A status-code consists of 3 DIGITS, was: '") + buffer[0] + buffer[1] + buffer[2],
                            "RFC 7230 Section 3.1.2",
                            "https://tools.ietf.org/html/rfc7230#section-3.1.2");
        // TODO Maybe mention DIGIT spec?
    }

    response.statusCode = static_cast<std::uint16_t>(
            ConvertCharToSingleDigit(buffer[0]) * 100 +
            ConvertCharToSingleDigit(buffer[1]) * 10 +
            ConvertCharToSingleDigit(buffer[2]));

    char next = connection->ReadChar();
    if (next != ' ') {
        throw HTTPException(__PRETTY_FUNCTION__ , std::string("A status-code consists of 3 DIGITS and is followed by a 0x20 SPACE, was: '") + next,
                            "RFC 7230 Section 3.1.2",
                            "https://tools.ietf.org/html/rfc7230#section-3.1.2");
    }
}

bool IsValidReasonPhraseCharacter(char c) {
    return c == '\t' || (c >= 0x20 && c <= 0x7E);
}

void
HTTPResponseReader::ReadReasonPhrase() {
    bool lastWasCR = false;
    std::vector<char> buffer;

    while (true) {
        char c = connection->ReadChar();

        if (lastWasCR) {
            if (c == '\n') {
                break;
            } else if (strict) {
                throw HTTPException(__PRETTY_FUNCTION__ , std::string("A reason-phrase should be ended with a CRLF, not a CR, LF and no dangling CR may be present"),
                                    "RFC 7230 Section 3.1.2",
                                    "https://tools.ietf.org/html/rfc7230#section-3.1.2");
            } else {
                lastWasCR = false;
                continue;
            }
        }

        if (c == '\n') {
            if (strict) {
                throw HTTPException(__PRETTY_FUNCTION__ , std::string("A reason-phrase should be ended with a CRLF, not a CR, LF and no dangling CR may be present"),
                                    "RFC 7230 Section 3.1.2",
                                    "https://tools.ietf.org/html/rfc7230#section-3.1.2");
            } else {
                break;
            }
        }

        if (c == '\r') {
            lastWasCR = true;
            continue;
        }

        if (!IsValidReasonPhraseCharacter(c)) {
            std::stringstream stream;
            stream << "A reason-phrase may only contain a HTAB, SP or VCHAR, not a '" << c << "' 0x" << std::hex << static_cast<std::uint16_t>(c);
            throw HTTPException(__PRETTY_FUNCTION__ , stream.str(),
                                "RFC 7230 Section 3.1.2",
                                "https://tools.ietf.org/html/rfc7230#section-3.1.2");
        }

        buffer.push_back(c);
    }

    response.reasonPhrase = std::string(std::cbegin(buffer), std::cend(buffer));
}
