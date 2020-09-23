/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <functional>

template<typename FunctionType = std::nullptr_t>
class StreamWrapper {
public:
    template<typename A = const char *, typename B = const char *> inline
    StreamWrapper(const A &prefix = "", const B &suffix = "\n", std::ostream &stream = std::cout, FunctionType destructorHookFunction=nullptr)
        : stream(stream), suffix(suffix), destructorHookFunction(destructorHookFunction) {
        stream << prefix;
    }

    inline
    ~StreamWrapper() {
        stream << suffix;
        if constexpr (std::is_invocable<FunctionType>::value) {
            std::invoke(destructorHookFunction);
        } else if constexpr (std::is_invocable<FunctionType, StreamWrapper *>::value) {
            std::invoke(destructorHookFunction, this);
        }
    }

    template<typename T>
    StreamWrapper &operator<<(const T &t) {
        stream << t;
        return *this;
    }

private:
    std::ostream &stream;
    const std::string suffix;
    const FunctionType destructorHookFunction;
};
