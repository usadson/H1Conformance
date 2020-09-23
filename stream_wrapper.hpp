/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <functional>

template<typename FunctionType = std::nullptr_t, typename StreamType = std::ostream, typename UserDataType = std::nullptr_t>
class StreamWrapper {
public:
    template<typename A = const char *, typename B = const char *> inline
    StreamWrapper(StreamType *stream=nullptr, A prefix = "", B suffix = "\n", FunctionType destructorHookFunction=nullptr, bool ownsStream=false, UserDataType userData = nullptr)
        : stream(stream), suffix(suffix), destructorHookFunction(destructorHookFunction), owning(ownsStream), userData(userData) {
        *stream << prefix;
    }

    inline
    ~StreamWrapper() {
        *stream << suffix;

        if constexpr (std::is_invocable<FunctionType>::value) {
            std::invoke(destructorHookFunction);
        } else if constexpr (std::is_invocable<FunctionType, StreamType *>::value) {
            std::invoke(destructorHookFunction, stream);
        } else if constexpr (std::is_invocable<FunctionType, StreamType *, UserDataType>::value) {
            std::invoke(destructorHookFunction, stream, userData);
        } else if constexpr (std::is_invocable<FunctionType, UserDataType>::value) {
            std::invoke(destructorHookFunction, userData);
        }

        if (owning) {
            delete stream;
        }
    }

    template<typename T>
    StreamWrapper &operator<<(const T &t) {
        *stream << t;
        return *this;
    }

    [[nodiscard]] constexpr inline StreamType *
    Stream() noexcept {
        return stream;
    }

    [[nodiscard]] constexpr inline const StreamType *
    Stream() const noexcept {
        return stream;
    }

private:
    StreamType *stream;
    bool owning = false;
    const std::string suffix;
    const FunctionType destructorHookFunction;
    const UserDataType userData;
};
