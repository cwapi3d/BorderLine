//
// Created by MichaelBrunner on 13/12/2025.
//

#pragma once

#include <cwapi3d/geometry/Vec3D.h>
#include <optional>

namespace borderline::edgeprofile::utility
{

struct AlignedVectors
{
    CwAPI3D::Geometry::Vec3D xl;
    CwAPI3D::Geometry::Vec3D yl;
};

class VectorAlignmentUtility
{
public:
    /**
     * @brief Aligns master element vectors onto the panel's reference plane.
     * 
     * @param panelXl Panel's local X-axis
     * @param panelYl Panel's local Y-axis
     * @param panelZl Panel's local Z-axis
     * @param masterZl Master element's local Z-axis (reference plane normal)
     * @param masterXl Master element's local X-axis
     * @return Aligned X and Y vectors, or std::nullopt if alignment fails
     */
    [[nodiscard]] static std::optional<AlignedVectors> alignMasterVectorsOnReferencePlane(
        const CwAPI3D::Geometry::Vec3D& panelXl,
        const CwAPI3D::Geometry::Vec3D& panelYl,
        const CwAPI3D::Geometry::Vec3D& panelZl,
        const CwAPI3D::Geometry::Vec3D& masterZl,
        const CwAPI3D::Geometry::Vec3D& masterXl);

private:
    static constexpr double COLLINEAR_TOLERANCE = 1e-4;
    static constexpr double ZERO_VECTOR_TOLERANCE = 1e-6;

    [[nodiscard]] static bool areCollinear(
        const CwAPI3D::Geometry::Vec3D& v1, 
        const CwAPI3D::Geometry::Vec3D& v2);

    [[nodiscard]] static bool isZeroVector(const CwAPI3D::Geometry::Vec3D& v);
};

} // namespace borderline::edgeprofile::utility
