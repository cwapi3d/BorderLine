#pragma once

#include <map>
#include "types/types.hh"
#include "EdgeLine.hh"
#include "EdgeOffsetConfig.hh"
#include "cwapi3d/geometry/CoordinateFrame3D.h"

namespace borderline::edgeprofile::ports {

class IEdgeOffsetStrategy {
public:
    virtual ~IEdgeOffsetStrategy() = default;

    virtual void applyOffsets(std::map<EEdgePosition, EdgeLine>& edges,
                              const CwAPI3D::Geometry::CoordinateFrame3D& coordFrame,
                              const EdgeOffsetConfig& offsetConfig) const = 0;
};

}
