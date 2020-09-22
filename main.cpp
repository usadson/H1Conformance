/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>

#include "connection/exception.hpp"
#include "connection/insecure_connection.hpp"

int main() {
    try {
        auto con = InsecureConnection("128.199.37.70");
    } catch (const ConnectionException &exception) {
        std::cerr << "[ConnectionError] (" << exception.Stage() << ") " << exception.Message() << '\n';
    }

    return 0;
}
