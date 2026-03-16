//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <borderline/edge_profile/edge_profile_export.h>
#include <cwapi3d/composite/Component.h>
#include <cwapi3d/composite/ComponentVisitor.h>

#include "cwapi3d/geometry/BoundingBox.h"
#include "types/types.hh"

namespace borderline::edgeprofile
{
namespace ports
{
class IGeometryControlPort;
}

class EDGE_PROFILE_EXPORT CabinetAggregate
{
public:
    explicit CabinetAggregate(const std::shared_ptr<ports::IGeometryControlPort> &aGeometryControlPort,
                              const ElementID &aElementId);

    [[nodiscard]] auto getElementID() const -> ElementID
    {
        return mElementID;
    }
    [[nodiscard]] auto getXDir() const -> CwAPI3D::Geometry::Vec3D;
    [[nodiscard]] auto getYDir() const -> CwAPI3D::Geometry::Vec3D;
    [[nodiscard]] auto getZDir() const -> CwAPI3D::Geometry::Vec3D;

private:
    std::shared_ptr<ports::IGeometryControlPort> mGeometryControlPort;
    ElementID mElementID;
};
}
