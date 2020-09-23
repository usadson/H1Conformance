/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite.hpp"

class HostHeader : public Suite {
public:
    HostHeader(const Configuration &configuration, const std::string &collectionName) :
            Suite(configuration, collectionName, "LackingHostHeader") {
    }

    void
    Run() override;

private:
    void
    RunWithout();

    void
    RunWith();

    void
    RunWithMultiple();

};

