//
// Created by MichaelBrunner on 09/12/2025.
//

#include "CabinetElementGeometry.hh"

#include <utility>

namespace borderline::edgeprofile {
CabinetElementGeometry::CabinetElementGeometry(const ElementID elementId,
                                               std::shared_ptr<ports::IGeometryControlPort> geometryControlPort)
    : mElementID(elementId), mGeometryControlPort(std::move(geometryControlPort)) {}

CwAPI3D::Geometry::CoordinateFrame3D CabinetElementGeometry::getCoordinateFrame() const {
  return CwAPI3D::Geometry::CoordinateFrame3D{
      mGeometryControlPort->getAxisOriginPt(mElementID), mGeometryControlPort->getLocalXDir(mElementID),
      mGeometryControlPort->getLocalYDir(mElementID), mGeometryControlPort->getLocalZDir(mElementID)};
}

double CabinetElementGeometry::getLength() const { return mGeometryControlPort->getLength(mElementID); }

double CabinetElementGeometry::getWidth() const { return mGeometryControlPort->getWidth(mElementID); }

double CabinetElementGeometry::getHeight() const { return mGeometryControlPort->getHeight(mElementID); }

CwAPI3D::Geometry::Point3D CabinetElementGeometry::getAxisOriginPt() const {
  return mGeometryControlPort->getAxisOriginPt(mElementID);
}

CwAPI3D::Geometry::Point3D CabinetElementGeometry::getAxisEndPt() const {
  return mGeometryControlPort->getAxisEndPt(mElementID);
}

CwAPI3D::Geometry::Point3D CabinetElementGeometry::getAxisHeightPt() const {
  return mGeometryControlPort->getAxisHeightPt(mElementID);
}
}  // namespace borderline::edgeprofile
// borderline
