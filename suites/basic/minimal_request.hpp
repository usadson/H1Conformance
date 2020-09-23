/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite.hpp"

class MinimalRequest : public Suite {
public:
    MinimalRequest(const Configuration &configuration, const std::string &collectionName) :
        Suite(configuration, collectionName, "MinimalRequest") {
    }

    void
    Run() override;

};
