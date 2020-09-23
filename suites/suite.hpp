/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <memory>
#include <sstream>

#include "../connection/insecure_connection.hpp"
#include "../configuration.hpp"
#include "../stream_wrapper.hpp"

inline void
FailureHook(std::stringstream *stream) {
    std::cout << "Test?\n";
}

class Suite {
public:
    Suite(const Configuration &configuration, const std::string &collectionName, const char *suiteName) noexcept :
        configuration(configuration), collectionName(collectionName), suiteName(suiteName) {
    }

    virtual
    ~Suite() = default;

    virtual void
    Run() = 0;

    virtual inline void
    Prepare() {
        connection = std::make_unique<InsecureConnection>(configuration.address, configuration.port);
    }

    // Failure() should be called when a suite doesn't pass.
    // Use the StreamWrapper<...> to tell the user what went wrong.

    inline StreamWrapper<void (*)(std::stringstream *), std::stringstream>
    Failure() {
        return StreamWrapper("", "", &FailureHook, new std::stringstream, true);
    }

    inline StreamWrapper<std::nullptr_t>
    Warning() {
        return StreamWrapper(std::string("[Warning] (") + collectionName + '/' + suiteName + ") ");
    }

protected:
    std::unique_ptr<Connection> connection;
    const Configuration &configuration;

    const std::string &collectionName;
    const char *suiteName;
};
