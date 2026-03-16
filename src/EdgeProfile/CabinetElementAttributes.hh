//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include <memory>
#include <borderline/edge_profile/edge_profile_export.h>

#include <optional>
#include "types/types.hh"

namespace borderline::edgeprofile
{
namespace ports
{
class IAttributeControlPort;
}

class EDGE_PROFILE_EXPORT CabinetElementAttributes
{
public:
    CabinetElementAttributes(const std::shared_ptr<ports::IAttributeControlPort> &aAttributeControlPort,
        const ElementID &aElementId,
        const std::optional<Number> &aPreviousProductionNumber);
    [[nodiscard]] Number getProductionNumber() const;
    [[nodiscard]] Number getAssemblyNumber() const;
    void setTemporaryProductionNumber(Number number);
    void resetTemporaryProductionNumber();

private:
    std::shared_ptr<ports::IAttributeControlPort> mAttributeControlPort;
    ElementID mElementID;
    std::optional<Number> mPreviousProductionNumber{std::nullopt};
};
}
