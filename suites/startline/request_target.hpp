/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include "../suite.hpp"
#include "../../http/response.hpp"

class RequestTarget : public Suite {
public:
    RequestTarget(Configuration &configuration, const std::string &collectionName) :
            Suite(configuration, collectionName, "RequestTarget") {
    }

    void
    Run() override;

private:
    void
    RunAsteriskInNonOptions();

    void
    RunAuthorityInNonConnect();

    void
    RunInvalidAbsolutePath();

    void
    RunValidAbsoluteWithQuery();

    void
    RunValidOriginWithQuery();

    void
    RunValidAbsolutePath();

};
