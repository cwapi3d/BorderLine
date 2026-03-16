#include "StandardTriangleStrategy.hh"

#include <cwapi3d/geometry/Vec3D.h>

namespace borderline::adapter::cadwork {

edgeprofile::ports::TrianglePoints StandardTriangleStrategy::calculatePoints(const CwAPI3D::Geometry::Point3D& center,
                                                                             const CwAPI3D::Geometry::Vec3D& xDir,
                                                                             const CwAPI3D::Geometry::Vec3D& yDir,
                                                                             const edgeprofile::EEdgeSymbolSize size,
                                                                             const double minSize) const {
    const double length = getBaseLength(size, minSize);

    // Optimized math:
    // P1 = C + X*L/2 - Y*L/3
    // P2 = C - X*L/2 - Y*L/3
    // P3 = C + Y*(2L/3)

    const double oneThirdL = length / 3.0;
    const double twoThirdsL = 2.0 * oneThirdL;
    const double halfL = length / 2.0;

    const auto xOffset = xDir * halfL;
    const auto yOffsetMinus = yDir * oneThirdL;
    const auto yOffsetPlus = yDir * twoThirdsL;

    return {
        center + xOffset - yOffsetMinus,  // P1
        center - xOffset - yOffsetMinus,  // P2
        center + yOffsetPlus              // P3
    };
}

double StandardTriangleStrategy::getBaseLength(const edgeprofile::EEdgeSymbolSize size, const double minSize) {
    switch (size) {
        case edgeprofile::EEdgeSymbolSize::SMALL:
            return minSize * 0.2;
        case edgeprofile::EEdgeSymbolSize::MEDIUM:
            return minSize * 0.6;
        case edgeprofile::EEdgeSymbolSize::LARGE:
            return minSize * 0.9;
        default:
            return minSize * 0.6;
    }
}

}  // namespace borderline::adapter::cadwork
