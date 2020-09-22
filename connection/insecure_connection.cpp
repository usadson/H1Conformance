/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "insecure_connection.hpp"

#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "exception.hpp"

InsecureConnection::InsecureConnection(const char *address, std::uint16_t port)
        : Connection(address, port) {
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        throw ConnectionException("connecting", "Failed to create a socket");
    }

    struct sockaddr_in remote{0};
    remote.sin_family = AF_INET;
    remote.sin_port = htons(port);
    remote.sin_addr.s_addr = inet_addr(address);

    if (connect(sockfd, (struct sockaddr *) &remote, sizeof(remote)) == -1) {
        switch (errno) {
            case ECONNREFUSED:
                throw ConnectionException("connecting", "Connection refused");
            case ENETUNREACH:
                throw ConnectionException("connecting", "Network unreachable");
            default:
                std::cout << "errno=" << errno << '\n';
                throw ConnectionException("connecting", "Unable to reach (generic)");
        }
    }
}

InsecureConnection::~InsecureConnection() noexcept {
    close(sockfd);
}

char
InsecureConnection::ReadChar() {
    char ret;

    if (read(sockfd, &ret, 1) != 1) {
        throw ConnectionException("reading", "Failed to read(2) 1 char");
    }

    return ret;
}

void
InsecureConnection::Read(char *buffer, std::size_t count) {
    do {
        int ret = read(sockfd, buffer, count);

        if (ret == -1) {
            throw ConnectionException("reading", "Failed to read(2)");
        }

        count -= ret;
        buffer += ret;
    } while (count > 0);
}

void
InsecureConnection::Write(const char *buffer, std::size_t count) {
    do {
        int ret = write(sockfd, buffer, count);

        if (ret == -1) {
            throw ConnectionException("writing", "Failed to write(2)");
        }

        count -= ret;
        buffer += ret;
    } while (count > 0);
}
