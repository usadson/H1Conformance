/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <iostream>

#include "configuration.hpp"
#include "suites/basic/basic_collection.hpp"
#include "suites/startline/startline_collection.hpp"

int main() {
    Configuration configuration{ "127.0.0.1", 80, "localhost" };

    auto collection = BasicSuiteCollection(configuration);
    auto collection2 = StartLineCollection(configuration);

    collection.RunSuites();
    collection2.RunSuites();

    return 0;
}
