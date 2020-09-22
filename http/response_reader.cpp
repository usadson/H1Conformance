/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "response_reader.hpp"

#include <array>
#include <vector>

HTTPResponse
HTTPResponseReader::Read() {
    ReadVersion();
    return response;
}

void
HTTPResponseReader::ReadVersion() {
    if (strict) {
        std::array<char, 9> buffer;
        connection->Read(buffer);

        if (buffer[0] != 'H' || buffer[1] != 'T' || buffer[2] != 'T' || buffer[3] != 'P' || buffer[4] != '/' || buffer[6] != '.') {
            throw std::exception();
        }

        if (buffer[5] != '1' || (buffer[7] != '0' && buffer[7] != '1')) {
            throw std::exception();
        }

        if (buffer[8] != ' ') {
            throw std::exception();
        }
        response.version = std::string(std::begin(buffer), std::end(buffer));
    } else {
        std::vector<char> buffer;
        char c;
        while ((c = connection->ReadChar()) != ' ') {
            buffer.push_back(c);
        }
        response.version = std::string(std::begin(buffer), std::end(buffer));
    }
}
