#include "StandardEdgeOffsetStrategy.hh"

#include "CabinetElementAttributes.hh"
#include "cwapi3d/geometry/Point3D.h"

namespace {

using namespace borderline::edgeprofile;

double getEdgeTypeOffset(const EEdgeType edgeType, const EdgeOffsetConfig &offsetConfig) {
    switch (edgeType) {
        case EEdgeType::COUNTERFLOW:
            return offsetConfig.getCounterflowDistance();
        case EEdgeType::CONTINOUS:
            return offsetConfig.getContinousDistance();
        case EEdgeType::MITER:
            return offsetConfig.getMiterDistance();
        default:
            return 0.0;
    }
}

CwAPI3D::Geometry::Point3D calculateBottomOffset(const EEdgeVerticeID pointId, const EEdgeType edgeType,
                                                 const double generalOffset, const double edgeTypeOffset) {
    double xOffset = generalOffset;
    double yOffset = 0.0;
    if (pointId == EEdgeVerticeID::PT520) {
        yOffset = generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? edgeTypeOffset : -edgeTypeOffset);
    } else if (pointId == EEdgeVerticeID::PT508) {
        yOffset = -generalOffset + edgeTypeOffset;
    }
    return {xOffset, yOffset, 0.0};
}

CwAPI3D::Geometry::Point3D calculateLeftOffset(const EEdgeVerticeID pointId, const EEdgeType edgeType,
                                               const double generalOffset, const double edgeTypeOffset) {
    double xOffset = 0.0;
    double yOffset = -generalOffset;
    if (pointId == EEdgeVerticeID::PT508) {
        xOffset = generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? edgeTypeOffset : -edgeTypeOffset);
    } else if (pointId == EEdgeVerticeID::PT512) {
        xOffset = -generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? -edgeTypeOffset : edgeTypeOffset);
    }
    return {xOffset, yOffset, 0.0};
}

CwAPI3D::Geometry::Point3D calculateTopOffset(const EEdgeVerticeID pointId, const EEdgeType edgeType,
                                              const double generalOffset, const double edgeTypeOffset) {
    double xOffset = -generalOffset;
    double yOffset = 0.0;
    if (pointId == EEdgeVerticeID::PT512) {
        yOffset = -generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? -edgeTypeOffset : edgeTypeOffset);
    } else if (pointId == EEdgeVerticeID::PT516) {
        yOffset = generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? edgeTypeOffset : -edgeTypeOffset);
    }
    return {xOffset, yOffset, 0.0};
}

CwAPI3D::Geometry::Point3D calculateRightOffset(const EEdgeVerticeID pointId, const EEdgeType edgeType,
                                                const double generalOffset, const double edgeTypeOffset) {
    double xOffset = 0.0;
    double yOffset = generalOffset;
    if (pointId == EEdgeVerticeID::PT516) {
        xOffset = -generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? -edgeTypeOffset : edgeTypeOffset);
    } else if (pointId == EEdgeVerticeID::PT520) {
        xOffset = generalOffset + (edgeType == EEdgeType::COUNTERFLOW ? edgeTypeOffset : -edgeTypeOffset);
    }
    return {xOffset, yOffset, 0.0};
}

CwAPI3D::Geometry::Point3D calculateLocalOffset(const EEdgePosition edgePosition, const EEdgeVerticeID pointId,
                                                const EEdgeType edgeType, const EdgeOffsetConfig &offsetConfig) {
    const double edgeTypeOffset = getEdgeTypeOffset(edgeType, offsetConfig);
    const double generalOffset = offsetConfig.getGeneralOffset();

    switch (edgePosition) {
        case EEdgePosition::BOTTOM:
            return calculateBottomOffset(pointId, edgeType, generalOffset, edgeTypeOffset);
        case EEdgePosition::LEFT:
            return calculateLeftOffset(pointId, edgeType, generalOffset, edgeTypeOffset);
        case EEdgePosition::TOP:
            return calculateTopOffset(pointId, edgeType, generalOffset, edgeTypeOffset);
        case EEdgePosition::RIGHT:
            return calculateRightOffset(pointId, edgeType, generalOffset, edgeTypeOffset);
        default:
            return {0.0, 0.0, 0.0};
    }
}

CwAPI3D::Geometry::Point3D applyOffsetToPoint(const EdgeProfilePositionData &point, const EEdgePosition edgePosition,
                                              const CwAPI3D::Geometry::CoordinateFrame3D &coordFrame,
                                              const EdgeOffsetConfig &offsetConfig) {
    const auto worldPos = point.getPosition();
    const auto localPos = coordFrame.transformToLocal(worldPos);

    const auto offset = calculateLocalOffset(edgePosition, point.getEdgeVerticeID(), point.getEdgeType(), offsetConfig);

    const auto newLocalPos
        = CwAPI3D::Geometry::Point3D(localPos.x() + offset.x(), localPos.y() + offset.y(), localPos.z() + offset.z());

    return coordFrame.transformToWorld(newLocalPos);
}
}  // namespace

namespace borderline::adapter::cadwork {

void StandardEdgeOffsetStrategy::applyOffsets(std::map<EEdgePosition, EdgeLine> &edges,
                                              const CwAPI3D::Geometry::CoordinateFrame3D &coordFrame,
                                              const EdgeOffsetConfig &offsetConfig) const {
    for (auto &[position, edge] : edges) {
        if (!edge.hasEdge()) {
            continue;
        }

        // Apply offsets directly to edge points (explicit, no side effects)
        const auto &startPoint = edge.getStartPoint();
        const auto &endPoint = edge.getEndPoint();

        auto newStartPos = applyOffsetToPoint(startPoint, position, coordFrame, offsetConfig);
        auto newEndPos = applyOffsetToPoint(endPoint, position, coordFrame, offsetConfig);

        edge.updateStartPointPosition(newStartPos);
        edge.updateEndPointPosition(newEndPos);
    }
}

}  // namespace borderline::adapter::cadwork
