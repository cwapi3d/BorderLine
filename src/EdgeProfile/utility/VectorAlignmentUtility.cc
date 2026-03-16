//
// Created by MichaelBrunner on 13/12/2025.
//

#include "VectorAlignmentUtility.hh"

namespace borderline::edgeprofile::utility
{

std::optional<AlignedVectors> VectorAlignmentUtility::alignMasterVectorsOnReferencePlane(
    const CwAPI3D::Geometry::Vec3D& panelXl,
    const CwAPI3D::Geometry::Vec3D& panelYl,
    const CwAPI3D::Geometry::Vec3D& panelZl,
    const CwAPI3D::Geometry::Vec3D& masterZl,
    const CwAPI3D::Geometry::Vec3D& masterXl)
{
    const CwAPI3D::Geometry::Vec3D globalZ(0.0, 0.0, 1.0);
    
    // Master Z must be collinear with global Z (reference plane requirement)
    if (!areCollinear(masterZl, globalZ)) {
        return std::nullopt;
    }

    // If panel Z is already aligned with master Z, use panel's own axes
    if (areCollinear(panelZl, masterZl)) {
        return AlignedVectors{panelXl, panelYl};
    }

    CwAPI3D::Geometry::Vec3D alignedXl;
    CwAPI3D::Geometry::Vec3D alignedYl;

    // Check all possible collinear cases between master Z and panel axes
    if (areCollinear(masterZl, panelXl)) {
        alignedYl = masterZl;
        alignedXl = panelYl;
    }
    else if (areCollinear(masterZl, panelYl)) {
        alignedYl = masterZl;
        alignedXl = panelXl;
    }
    else if (areCollinear(masterZl, panelXl * -1.0)) {
        alignedYl = masterZl;
        alignedXl = panelYl * -1.0;
    }
    else if (areCollinear(masterZl, panelYl * -1.0)) {
        alignedYl = masterZl;
        alignedXl = panelXl * -1.0;
    }
    else {
        // General case: compute orthogonal X-axis via cross product
        alignedYl = masterZl;
        alignedXl = masterZl.cross(panelXl);
    }

    // Validate result vectors
    if (isZeroVector(alignedXl) || isZeroVector(alignedYl)) {
        return std::nullopt;
    }

    return AlignedVectors{alignedXl, alignedYl};
}

bool VectorAlignmentUtility::areCollinear(
    const CwAPI3D::Geometry::Vec3D& v1,
    const CwAPI3D::Geometry::Vec3D& v2)
{
    const auto cross = v1.cross(v2);
    return cross.length() < COLLINEAR_TOLERANCE;
}

bool VectorAlignmentUtility::isZeroVector(const CwAPI3D::Geometry::Vec3D& v)
{
    return v.length() < ZERO_VECTOR_TOLERANCE;
}

} // namespace borderline::edgeprofile::utility
