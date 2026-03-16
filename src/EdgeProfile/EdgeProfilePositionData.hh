//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include "cwapi3d/geometry/Point3D.h"
#include "utility/utility_types.hh"

namespace borderline::edgeprofile
{
class EdgeProfilePositionData
{
public:
    EdgeProfilePositionData(const CwAPI3D::Geometry::Point3D &aPosition, const EEdgeVerticeID aEdgeId, const EEdgeType aEdgeType)
        : mPosition(aPosition),
          mEdgeVerticeID(aEdgeId),
          mEdgeType(aEdgeType)
    {
    }

    EdgeProfilePositionData() = default;

    auto getPosition() -> CwAPI3D::Geometry::Point3D &
    {
        return mPosition;
    }
    [[nodiscard]] auto getPosition() const -> const CwAPI3D::Geometry::Point3D &
    {
        return mPosition;
    }
    void setPosition(const CwAPI3D::Geometry::Point3D &position)
    {
        mPosition = position;
    }

    [[nodiscard]] auto getEdgeVerticeID() const -> EEdgeVerticeID
    {
        return mEdgeVerticeID;
    }
    void setEdgeId(const EEdgeVerticeID edgeId)
    {
        mEdgeVerticeID = edgeId;
    }

    [[nodiscard]] auto getEdgeType() const -> EEdgeType
    {
        return mEdgeType;
    }
    void setEdgeType(const EEdgeType edgeType)
    {
        mEdgeType = edgeType;
    }

private:
    CwAPI3D::Geometry::Point3D mPosition;
    EEdgeVerticeID mEdgeVerticeID; // Assuming None exists or default initialization is desired
    EEdgeType mEdgeType{EEdgeType::NONE}; // Assuming NONE exists
};
}
