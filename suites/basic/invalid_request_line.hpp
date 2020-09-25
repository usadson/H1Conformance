/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <string>

#include "../suite.hpp"

class InvalidRequestLine : public Suite {
public:
    InvalidRequestLine(Configuration &configuration, const std::string &collectionName) :
            Suite(configuration, collectionName, "InvalidRequestLine") {
    }

    void
    Run() override;

private:
    [[nodiscard]] std::uint16_t
    Request(const std::string &request);

    void
    RunMethodValid();

    void
    RunMethodInvalid();

    void
    RunVersionOld();

    void
    RunVersionIncorrectCase();

};

