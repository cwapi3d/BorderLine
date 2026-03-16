#include "EdgeGeometryUtils.hh"

#include "ITriangleStrategy.hh"

namespace borderline::edgeprofile {

std::map<EEdgePosition, EdgeLine> EdgeGeometryUtils::createEdgeLines(const CabinetElementGeometry &geometry) {
    const auto length = geometry.getLength();
    const auto width = geometry.getWidth();
    const double halfWidth = width / 2.0;
    const auto &coordFrame = geometry.getCoordinateFrame();

    // Define corners in local coordinates
    // PT520: (0, -width/2, 0)
    // PT508: (0, width/2, 0)
    // PT512: (length, width/2, 0)
    // PT516: (length, -width/2, 0)

    const auto p520Local = CwAPI3D::Geometry::Point3D(0.0, -halfWidth, 0.0);
    const auto p508Local = CwAPI3D::Geometry::Point3D(0.0, halfWidth, 0.0);
    const auto p512Local = CwAPI3D::Geometry::Point3D(length, halfWidth, 0.0);
    const auto p516Local = CwAPI3D::Geometry::Point3D(length, -halfWidth, 0.0);

    const auto p520 = coordFrame.transformToWorld(p520Local);
    const auto p508 = coordFrame.transformToWorld(p508Local);
    const auto p512 = coordFrame.transformToWorld(p512Local);
    const auto p516 = coordFrame.transformToWorld(p516Local);

    std::map<EEdgePosition, EdgeLine> edges;

    // Edge 1 (BOTTOM): PT520 -> PT508
    EdgeLine edge1;
    edge1.setStartPoint({p520, EEdgeVerticeID::PT520, EEdgeType::CONTINOUS});
    edge1.setEndPoint({p508, EEdgeVerticeID::PT508, EEdgeType::CONTINOUS});
    edges[EEdgePosition::BOTTOM] = edge1;

    // Edge 2 (LEFT): PT508 -> PT512
    EdgeLine edge2;
    edge2.setStartPoint({p508, EEdgeVerticeID::PT508, EEdgeType::CONTINOUS});
    edge2.setEndPoint({p512, EEdgeVerticeID::PT512, EEdgeType::CONTINOUS});
    edges[EEdgePosition::LEFT] = edge2;

    // Edge 3 (TOP): PT512 -> PT516
    EdgeLine edge3;
    edge3.setStartPoint({p512, EEdgeVerticeID::PT512, EEdgeType::CONTINOUS});
    edge3.setEndPoint({p516, EEdgeVerticeID::PT516, EEdgeType::CONTINOUS});
    edges[EEdgePosition::TOP] = edge3;

    // Edge 4 (RIGHT): PT516 -> PT520
    EdgeLine edge4;
    edge4.setStartPoint({p516, EEdgeVerticeID::PT516, EEdgeType::CONTINOUS});
    edge4.setEndPoint({p520, EEdgeVerticeID::PT520, EEdgeType::CONTINOUS});
    edges[EEdgePosition::RIGHT] = edge4;

    return edges;
}

void EdgeGeometryUtils::projectEdgesToPlane(std::map<EEdgePosition, EdgeLine> &edges,
                                            const CwAPI3D::Geometry::CoordinateFrame3D &coordFrame,
                                            const double zValue) {
    for (auto &[position, edge] : edges | std::views::filter([](auto &&aPair) { return aPair.second.hasEdge(); })) {
        const auto &startPoint = edge.getStartPoint();
        const auto &endPoint = edge.getEndPoint();

        const auto projectedPoint = coordFrame.origin() + coordFrame.zAxis() * zValue;
        const auto plane = CwAPI3D::Geometry::Plane3D(projectedPoint, coordFrame.zAxis());
        const auto projectedStartPoint = plane.projectPoint(startPoint.getPosition());
        const auto projectedEndPoint = plane.projectPoint(endPoint.getPosition());

        edge.updateStartPointPosition(projectedStartPoint);
        edge.updateEndPointPosition(projectedEndPoint);
    }
}
ports::TrianglePoints EdgeGeometryUtils::projectTriangleToPlane(const ports::TrianglePoints &trianglePoints,
                                                                const CwAPI3D::Geometry::CoordinateFrame3D &coordFrame,
                                                                const double zValue) {
    const auto &[p1, p2, p3] = trianglePoints;
    const auto projectedPoint = coordFrame.origin() + coordFrame.zAxis() * zValue;
    const auto plane = CwAPI3D::Geometry::Plane3D(projectedPoint, coordFrame.zAxis());

    ports::TrianglePoints result;
    result.p1 = plane.projectPoint(p1);
    result.p2 = plane.projectPoint(p2);
    result.p3 = plane.projectPoint(p3);

    return result;
}

}  // namespace borderline::edgeprofile
