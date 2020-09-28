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

#include "suite_exception.hpp"

inline void
FailureHook(std::stringstream *stream, const Suite *suite);

class Suite {
public:
    Suite(Configuration &configuration, const std::string &collectionName, const char *suiteName) noexcept :
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

    inline auto
    Warning() {
        return StreamWrapper(&std::cout, std::string("[Warning] (") + collectionName + '/' + suiteName + ") ");
    }

    [[nodiscard]] inline std::string
    QualifiedName() const noexcept {
        std::stringstream stream;
        stream << collectionName << '.' << suiteName;

        for (const auto &section : sections) {
            stream << '.' << section;
        }

        return stream.str();
    }

    inline void
    PushSection(const char *s) noexcept {
        sections.push_back(s);
    }

    inline void
    PopSection() noexcept {
        sections.pop_back();
    }

protected:
    std::unique_ptr<Connection> connection;
    Configuration &configuration;

    const std::string &collectionName;
    const char *suiteName;
    std::vector<const char *> sections;
};

inline void
FailureHook(std::stringstream *stream, const Suite *suite) {
    throw SuiteFailureException(*suite, stream->str());
}

class SuiteSectionGuard {
public:
    inline
    SuiteSectionGuard(Suite &suite, const char *name) noexcept : suite(suite) {
        suite.PushSection(name);
    }

    ~SuiteSectionGuard() noexcept {
        suite.PopSection();
    }

private:
    Suite &suite;
};
