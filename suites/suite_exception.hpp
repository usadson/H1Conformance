/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <exception>
#include <string>
#include <sstream>

#include "suite.hpp"

class SuiteFailureException : public std::exception {
public:
    inline
    SuiteFailureException(const Suite &suite, std::string &&message) noexcept
            : suite(suite), message(message) {
    }

    [[nodiscard]] constexpr inline const std::string &
    Message() const noexcept {
        return message;
    }

    [[nodiscard]] constexpr inline const Suite &
    Suite() const noexcept {
        return suite;
    }

private:
    const class Suite &suite;
    const std::string message;
};
