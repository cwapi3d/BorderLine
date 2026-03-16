#include <cwapi3d/ICwAPI3DControllerFactory.h>
#include <cwapi3d/ICwAPI3DGeometryController.h>
#include <cwapi3d/geometry/Point3D.h>
#include <cwapi3d/geometry/Vec3D.h>

#include "GeometryControl.hh"

namespace borderline::adapter::cadwork
{
GeometryControl::GeometryControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory)
    : mFactory(factory)
{
    if (mFactory) {
        mGeometryController = mFactory->getGeometryController();
    }
}

auto GeometryControl::getLocalXDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getXL(element.get());
    return CwAPI3D::Geometry::Vec3D(mX, mY, mZ);
}

auto GeometryControl::getLocalYDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getYL(element.get());
    return CwAPI3D::Geometry::Vec3D(mX, mY, mZ);
}

auto GeometryControl::getLocalZDir(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Vec3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getZL(element.get());
    return CwAPI3D::Geometry::Vec3D(mX, mY, mZ);
}

auto GeometryControl::getAxisOriginPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getP1(element.get());
    return CwAPI3D::Geometry::Point3D(mX, mY, mZ);
}

auto GeometryControl::getAxisEndPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getP2(element.get());
    return CwAPI3D::Geometry::Point3D(mX, mY, mZ);
}

auto GeometryControl::getAxisHeightPt(edgeprofile::ElementID element) const -> CwAPI3D::Geometry::Point3D
{
    if (!mGeometryController) return {};
    const auto [mX, mY, mZ] = mGeometryController->getP3(element.get());
    return CwAPI3D::Geometry::Point3D(mX, mY, mZ);
}

double GeometryControl::getLength(edgeprofile::ElementID element) const
{
    if (!mGeometryController) return 0.0;
    return mGeometryController->getLength(element.get());
}

double GeometryControl::getWidth(edgeprofile::ElementID element) const
{
    if (!mGeometryController) return 0.0;
    return mGeometryController->getWidth(element.get());
}

double GeometryControl::getHeight(edgeprofile::ElementID element) const
{
    if (!mGeometryController) return 0.0;
    return mGeometryController->getHeight(element.get());
}

}
