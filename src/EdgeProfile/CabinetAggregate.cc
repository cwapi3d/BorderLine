//
// Created by MichaelBrunner on 09/12/2025.
//

#include "CabinetAggregate.hh"

#include "IGeometryControlPort.hh"

namespace borderline::edgeprofile
{
CabinetAggregate::CabinetAggregate(const std::shared_ptr<ports::IGeometryControlPort> &aGeometryControlPort,
                                   const ElementID &aElementId)
    : mGeometryControlPort(aGeometryControlPort),
      mElementID(aElementId)
{
}
auto CabinetAggregate::getXDir() const -> CwAPI3D::Geometry::Vec3D
{
    return mGeometryControlPort->getLocalXDir(mElementID);
}
auto CabinetAggregate::getYDir() const -> CwAPI3D::Geometry::Vec3D
{
    return mGeometryControlPort->getLocalYDir(mElementID);
}
auto CabinetAggregate::getZDir() const -> CwAPI3D::Geometry::Vec3D
{
    return mGeometryControlPort->getLocalZDir(mElementID);
}
}
