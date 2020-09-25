/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite_collection.hpp"

#include "method.hpp"
#include "version.hpp"
#include "request_target.hpp"

class StartLineCollection : public SuiteCollection {
public:
    StartLineCollection(Configuration &configuration)
            : SuiteCollection(configuration) {
        suites.push_back(std::make_unique<Method>(configuration, name));
        suites.push_back(std::make_unique<RequestTarget>(configuration, name));
        suites.push_back(std::make_unique<Version>(configuration, name));
    }

    ~StartLineCollection() = default;

private:
    const std::string name = "StartLineCollection";
};
