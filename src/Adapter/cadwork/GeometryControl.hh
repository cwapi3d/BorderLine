#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>
#include "IGeometryControlPort.hh"

namespace CwAPI3D::Geometry
{
class Point3D;
class Vec3D;
}

namespace CwAPI3D::Interfaces
{
class ICwAPI3DControllerFactory;
class ICwAPI3DGeometryController;
}

namespace borderline::adapter::cadwork
{
class CADWORK_ADAPTER_EXPORT GeometryControl: public edgeprofile::ports::IGeometryControlPort
{
public:
    explicit GeometryControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory);
    ~GeometryControl() override = default;

    [[nodiscard]] auto getLocalXDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D override;
    [[nodiscard]] auto getLocalYDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D override;
    [[nodiscard]] auto getLocalZDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D override;
    [[nodiscard]] auto getAxisOriginPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D override;
    [[nodiscard]] auto getAxisEndPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D override;
    [[nodiscard]] auto getAxisHeightPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D override;

   [[nodiscard]]  double getLength(edgeprofile::ElementID element) const override;
   [[nodiscard]]  double getWidth(edgeprofile::ElementID element) const override;
   [[nodiscard]]  double getHeight(edgeprofile::ElementID element) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory* mFactory{nullptr};
    CwAPI3D::Interfaces::ICwAPI3DGeometryController *mGeometryController{nullptr};
};
}
