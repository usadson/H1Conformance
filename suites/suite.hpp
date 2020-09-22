/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <memory>

#include "../connection/insecure_connection.hpp"
#include "../configuration.hpp"

class Suite {
public:
    Suite(const Configuration &configuration) noexcept :
        configuration(configuration) {
    }

    virtual
    ~Suite() = default;

    virtual void
    Run() = 0;

    virtual inline void
    Prepare() {
        connection = std::make_unique<InsecureConnection>(configuration.address, configuration.port);
    }

protected:
    std::unique_ptr<Connection> connection;
    const Configuration &configuration;
};
