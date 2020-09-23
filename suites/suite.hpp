/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <memory>
#include <sstream>

#include "../connection/insecure_connection.hpp"
#include "../configuration.hpp"
#include "../stream_wrapper.hpp"

class Suite;

class SuiteFailureException : public std::exception {
public:
    inline
    SuiteFailureException(const Suite &suite, std::string &&message) noexcept
        : suite(suite), message(message) {
    }

    [[nodiscard]] constexpr inline const std::string &
    Message() const noexcept {
        return message;
    }

    [[nodiscard]] constexpr inline const Suite &
    Suite() const noexcept {
        return suite;
    }

private:
    const class Suite &suite;
    const std::string message;
};

inline void
FailureHook(std::stringstream *stream, const Suite *suite) {
    throw SuiteFailureException(*suite, stream->str());
}

class Suite {
public:
    Suite(const Configuration &configuration, const std::string &collectionName, const char *suiteName) noexcept :
        configuration(configuration), collectionName(collectionName), suiteName(suiteName) {
    }

    virtual inline void
    Reconnect() {
        Prepare();
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

    inline StreamWrapper<void (*)(std::stringstream *, const Suite *), std::stringstream, Suite *>
    Failure() {
        return StreamWrapper(new std::stringstream, "", "", &FailureHook, true, this);
    }

    inline StreamWrapper<std::nullptr_t>
    Warning() {
        return StreamWrapper(&std::cout, std::string("[Warning] (") + collectionName + '/' + suiteName + ") ");
    }

    [[nodiscard]] constexpr inline const std::string &
    CollectionName() const noexcept {
        return collectionName;
    }

    [[nodiscard]] constexpr inline const char *
    Name() const noexcept {
        return suiteName;
    }

protected:
    std::unique_ptr<Connection> connection;
    const Configuration &configuration;

    const std::string &collectionName;
    const char *suiteName;
};
