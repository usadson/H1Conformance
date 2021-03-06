/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite_collection.hpp"

#include "host_header.hpp"
#include "minimal_request.hpp"

class BasicSuiteCollection : public SuiteCollection {
public:
    explicit
    BasicSuiteCollection(Configuration &configuration)
        : SuiteCollection(configuration) {
        suites.push_back(std::make_unique<HostHeader>(configuration, name));
        suites.push_back(std::make_unique<MinimalRequest>(configuration, name));
    }

    ~BasicSuiteCollection() override = default;

private:
    const std::string name = "BasicSuite";
};
