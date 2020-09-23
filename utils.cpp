/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "utils.hpp"

#include <algorithm>

char
Utils::GenerateTChar() noexcept {
    static const char disallowedCharacters[] = "\"(),/:;<=>?@[\\]{}";
    while (true) {
        char c = vcharDistribution(engine);
        if (std::find(std::cbegin(disallowedCharacters), std::cend(disallowedCharacters), c) == std::cend(disallowedCharacters)) {
            return c;
        }
    }
}

std::string
Utils::GenerateToken(std::size_t maxLength) noexcept {
    std::string buffer(' ', maxLength);
    std::generate(std::begin(buffer), std::end(buffer), [this]() {
        return GenerateTChar();
    });
    return std::string(std::begin(buffer), std::end(buffer));
}

std::string
Utils::GenerateRandomLengthToken(std::size_t maxLength) noexcept {
    return GenerateToken(std::uniform_int_distribution<std::size_t>(0, maxLength)(engine));
}
