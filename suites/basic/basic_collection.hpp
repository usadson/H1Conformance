/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite_collection.hpp"

class BasicSuiteCollection : public SuiteCollection {
public:
    BasicSuiteCollection(const Configuration &configuration)
        : SuiteCollection(configuration) {

    }

    ~BasicSuiteCollection() = default;
};