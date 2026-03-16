//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once


#include "types/types.hh"

namespace CwAPI3D::Geometry
{
class Vec3D;
class Point3D;
}

namespace borderline::edgeprofile::ports
{
class IGeometryControlPort
{
public:
    virtual ~IGeometryControlPort() = default;

    [[nodiscard]] virtual auto getLocalXDir(ElementID element) const -> CwAPI3D::Geometry::Vec3D = 0;
    [[nodiscard]] virtual auto getLocalYDir(ElementID element) const -> CwAPI3D::Geometry::Vec3D = 0;
    [[nodiscard]] virtual auto getLocalZDir(ElementID element) const -> CwAPI3D::Geometry::Vec3D = 0;
    [[nodiscard]] virtual auto getAxisOriginPt(ElementID element) const -> CwAPI3D::Geometry::Point3D = 0;
    [[nodiscard]] virtual auto getAxisEndPt(ElementID element) const -> CwAPI3D::Geometry::Point3D = 0;
    [[nodiscard]] virtual auto getAxisHeightPt(ElementID element) const -> CwAPI3D::Geometry::Point3D = 0;

    [[nodiscard]] virtual double getLength(ElementID element) const = 0;
    [[nodiscard]] virtual double getWidth(ElementID element) const = 0;
    [[nodiscard]] virtual double getHeight(ElementID element) const = 0;

};
}
