//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <borderline/edge_profile/edge_profile_export.h>
#include <memory>
#include <cwapi3d/geometry/CoordinateFrame3D.h>

#include "IGeometryControlPort.hh"

namespace borderline::edgeprofile
{
class EDGE_PROFILE_EXPORT CabinetElementGeometry
{
public:
    explicit CabinetElementGeometry(ElementID elementId,
                                    std::shared_ptr<ports::IGeometryControlPort> geometryControlPort);

    [[nodiscard]] CwAPI3D::Geometry::CoordinateFrame3D getCoordinateFrame() const;

    [[nodiscard]] double getLength() const;
    [[nodiscard]] double getWidth() const;
    [[nodiscard]] double getHeight() const;

    [[nodiscard]] CwAPI3D::Geometry::Point3D getAxisOriginPt() const;
    [[nodiscard]] CwAPI3D::Geometry::Point3D getAxisEndPt() const;
    [[nodiscard]] CwAPI3D::Geometry::Point3D getAxisHeightPt() const;

private:
    ElementID mElementID;
    std::shared_ptr<ports::IGeometryControlPort> mGeometryControlPort;
};
}
