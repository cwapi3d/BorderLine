//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include <borderline/edge_profile/edge_profile_export.h>
#include <cwapi3d/composite/Component.h>

#include <memory>
#include <utility>

#include "CabinetElementAttributes.hh"
#include "CabinetElementGeometry.hh"
#include "types/types.hh"

namespace borderline::edgeprofile {
class EDGE_PROFILE_EXPORT CabinetElement {
public:
    CabinetElement(const ElementID& aElementId, CabinetElementGeometry aGeometry, CabinetElementAttributes aAttributes)
        : mElementID(aElementId),
          mGeometry(std::move(aGeometry)),
          mAttributes(std::move(aAttributes)) {}
    [[nodiscard]] ElementID getElementID() const;
    [[nodiscard]] const CabinetElementGeometry& getGeometry() const { return mGeometry; }
    [[nodiscard]] const CabinetElementAttributes& getAttributes() const { return mAttributes; }

private:
    ElementID mElementID;
    CabinetElementGeometry mGeometry;
    CabinetElementAttributes mAttributes;
};
}  // namespace borderline::edgeprofile
