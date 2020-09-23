/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "host_header.hpp"

void
HostHeader::RunWithout() {

}

void
HostHeader::RunWith() {

}

void
HostHeader::RunWithMultiple() {

}

void
HostHeader::Run() {
    RunWith();
    Reconnect();
    RunWithout();
    Reconnect();
    RunWithMultiple();
}
