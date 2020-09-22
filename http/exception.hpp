/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <exception>
#include <string>

class HTTPException : public std::exception {
public:
    inline
    HTTPException(std::string tag, std::string message)
            : tag(tag), message(message) {
    }

    [[nodiscard]] inline constexpr const std::string &
    Message() const noexcept {
        return message;
    }

    [[nodiscard]] inline constexpr const std::string &
    Tag() const noexcept {
        return tag;
    }

private:
    const std::string tag;
    const std::string message;
};
