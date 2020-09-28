/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite.hpp"
#include "../../http/response.hpp"

class Version : public Suite {
public:
    Version(Configuration &configuration, const std::string &collectionName) :
            Suite(configuration, collectionName, "Version") {
    }

    void
    Run() override;

private:
    void
    RunOld();

    void
    RunIncorrectCase();

    void
    RunHigherMinor();

    void
    RunHigherMajor();

};
