//
// Created by MichaelBrunner on 10/12/2025.
//

#pragma once

#include <borderline/edge_profile/edge_profile_export.h>

#include "types/types.hh"

namespace borderline::edgeprofile
{
class EDGE_PROFILE_EXPORT NestingAggregate
{
public:
    explicit NestingAggregate(Number numberOfAssociatedNestingElement)
        : mNumberOfAssociatedNestingElement(numberOfAssociatedNestingElement)
    {
    }

    [[nodiscard]] auto getNumberOfAssociatedNestingElement() const -> Number;

private:
    Number mNumberOfAssociatedNestingElement;
};
}
