/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <vector>

#include "../configuration.hpp"
#include "suite.hpp"
#include "../connection/exception.hpp"
#include "../http/exception.hpp"

class SuiteCollection {
public:
    SuiteCollection(const Configuration &configuration) noexcept
        : configuration(configuration) {
    }

    virtual
    ~SuiteCollection() = default;

    inline virtual void
    RunSuites() {
        for (auto &suite : suites) {
            try {
                suite->Prepare();
                suite->Run();
            } catch (const ConnectionException &exception) {
                std::cerr << "[ConnectionException] (" << exception.Stage() << ") " << exception.Message() << '\n';
            } catch (const HTTPException &exception) {
                std::cerr << "[HTTPException] An HTTP error was detected."
                          << "\n\tTag: " << exception.Tag()
                          << "\n\tMessage: " << exception.Message()
                          << "\n\tSpecification: " << exception.Specification()
                          << "\n\tSpecification URL: " << exception.URL()
                          << '\n';
            }
        }
    }

protected:
    std::vector<std::unique_ptr<Suite>> suites;
    const Configuration &configuration;
};
