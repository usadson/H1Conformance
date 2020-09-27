/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <exception>
#include <string>

class ConnectionException : public std::exception {
public:
    inline
    ConnectionException(std::string stage, std::string message)
        : stage(std::move(stage)), message(std::move(message)) {
    }

    [[nodiscard]] inline constexpr const std::string &
    Message() const noexcept {
        return message;
    }

    [[nodiscard]] inline constexpr const std::string &
    Stage() const noexcept {
        return stage;
    }

private:
    // The stage is e.g. one of 'connecting', 'tls', 'reading', 'writing'
    const std::string stage;

    const std::string message;
};
