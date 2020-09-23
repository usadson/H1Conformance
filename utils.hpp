/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <array>
#include <random>

class Utils {
public:
    [[nodiscard]] char
    GenerateTChar() noexcept;

    [[nodiscard]] std::string
    GenerateToken(std::size_t maxLength=15) noexcept;

    [[nodiscard]] std::string
    GenerateRandomLengthToken(std::size_t min=1, std::size_t max=15) noexcept;

private:
    std::random_device device;
    std::default_random_engine engine{device()};
    std::uniform_int_distribution<char> vcharDistribution{0x20, 0x7E};
};
