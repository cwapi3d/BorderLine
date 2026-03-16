//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <algorithm>

namespace borderline::edgeprofile
{
template<typename T, typename Tag> class StrongType
{
    T value_;

public:
    StrongType() : value_{} {}

    explicit StrongType(T value)
        : value_(std::move(value))
    {
    }
    auto get() & -> T & { return value_; }
    auto get() const & -> const T & { return value_; }
    auto get() && -> T && { return std::move(value_); }

    explicit operator T() const { return value_; }
    auto operator<=>(const StrongType &) const = default;
};

using ElementID = StrongType<uint64_t, struct ElementIDTag>;
using NestingParentID = StrongType<uint32_t, struct NestingParentIDTag>;
using AttributeIdx = StrongType<uint32_t, struct AttributeIdxTag>;
using Number = StrongType<uint32_t, struct NumberTag>;

}
