/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <vector>

#include "../configuration.hpp"
#include "suite.hpp"

class SuiteCollection {
public:
    SuiteCollection(const Configuration &configuration) noexcept
        : configuration(configuration) {
    }

    virtual
    ~SuiteCollection() = default;

    inline virtual void
    RunSuites() {
        for (Suite &suite : suites) {
            suite.Prepare();
            suite.Run();
        }
    }

private:
    std::vector<Suite> suites;
    const Configuration &configuration;
};
