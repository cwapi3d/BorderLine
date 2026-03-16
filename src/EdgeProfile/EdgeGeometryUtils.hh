#pragma once

#include <map>
#include <ranges>
#include <borderline/edge_profile/edge_profile_export.h>
#include "types/types.hh"
#include "EdgeLine.hh"
#include "CabinetElementGeometry.hh"
#include "cwapi3d/geometry/CoordinateFrame3D.h"
#include "cwapi3d/geometry/Plane3D.h"

namespace borderline::edgeprofile {
namespace ports {
struct TrianglePoints;
}

class EDGE_PROFILE_EXPORT EdgeGeometryUtils {
public:
    static std::map<EEdgePosition, EdgeLine> createEdgeLines(const CabinetElementGeometry& geometry);
    
    static void projectEdgesToPlane(std::map<EEdgePosition, EdgeLine>& edges,
                                    const CwAPI3D::Geometry::CoordinateFrame3D& coordFrame,
                                    double zValue);

    static ports::TrianglePoints projectTriangleToPlane(const ports::TrianglePoints& trianglePoints,
                                                        const CwAPI3D::Geometry::CoordinateFrame3D& coordFrame,
                                                        double zValue);
};

}
