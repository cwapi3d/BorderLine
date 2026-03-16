//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <cwapi3d/composite/Component.h>

#include "EdgeProfilePositionData.hh"
#include "types/types.hh"

namespace borderline::edgeprofile
{
/**
 * @brief Represents an edge line of a cabinet panel with two edge positions
 * 
 * Domain entity that encapsulates edge line behavior and maintains invariants.
 * Provides explicit methods for modifications rather than exposing internal state.
 */
class EdgeLine
{
public:
    EdgeLine() = default;

    // Query methods (const, return by const reference or value)
    [[nodiscard]] auto getStartPoint() const -> const EdgeProfilePositionData& { return mStartPt; }
    [[nodiscard]] auto getEndPoint() const -> const EdgeProfilePositionData& { return mEndPt; }
    [[nodiscard]] auto getLineElementId() const -> ElementID { return mLineElementId; }
    [[nodiscard]] auto hasEdge() const -> bool { return mHasEdge; }

    // Command methods (explicit operations)
    void setStartPoint(const EdgeProfilePositionData& startPt) { mStartPt = startPt; }
    void setEndPoint(const EdgeProfilePositionData& endPt) { mEndPt = endPt; }
    void setLineElementId(const ElementID& elementID) { mLineElementId = elementID; }
    void setHasEdge(bool hasEdge) { mHasEdge = hasEdge; }
    
    /**
     * @brief Updates the edge type for the start point
     * @param edgeType The new edge type to apply
     */
    void updateStartPointEdgeType(EEdgeType edgeType)
    {
        mStartPt.setEdgeType(edgeType);
    }
    
    /**
     * @brief Updates the edge type for the end point
     * @param edgeType The new edge type to apply
     */
    void updateEndPointEdgeType(EEdgeType edgeType)
    {
        mEndPt.setEdgeType(edgeType);
    }
    
    /**
     * @brief Updates the position for the start point
     * @param position The new 3D position
     */
    void updateStartPointPosition(const CwAPI3D::Geometry::Point3D& position)
    {
        mStartPt.setPosition(position);
    }
    
    /**
     * @brief Updates the position for the end point
     * @param position The new 3D position
     */
    void updateEndPointPosition(const CwAPI3D::Geometry::Point3D& position)
    {
        mEndPt.setPosition(position);
    }

private:
    EdgeProfilePositionData mStartPt;
    EdgeProfilePositionData mEndPt;
    ElementID mLineElementId{0};
    bool mHasEdge{true};
};
}

