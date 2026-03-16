#pragma once

#include <cwapi3d/geometry/Point3D.h>


#include "utility/utility_types.hh"

namespace borderline::edgeprofile::ports {

struct TrianglePoints {
  CwAPI3D::Geometry::Point3D p1;
  CwAPI3D::Geometry::Point3D p2;
  CwAPI3D::Geometry::Point3D p3;
};

class ITriangleStrategy {
 public:
  virtual ~ITriangleStrategy() = default;

  [[nodiscard]] virtual TrianglePoints calculatePoints(const CwAPI3D::Geometry::Point3D& center,
                                                       const CwAPI3D::Geometry::Vec3D& xDir,
                                                       const CwAPI3D::Geometry::Vec3D& yDir, EEdgeSymbolSize size,
                                                       double minSize) const = 0;
};

}  // namespace borderline::edgeprofile::ports
