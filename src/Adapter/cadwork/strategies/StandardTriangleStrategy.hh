#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include <ports/ITriangleStrategy.hh>
#include <utility/utility_types.hh>

namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT StandardTriangleStrategy final : public edgeprofile::ports::ITriangleStrategy {
public:
    [[nodiscard]] edgeprofile::ports::TrianglePoints calculatePoints(const CwAPI3D::Geometry::Point3D& center,
                                                                     const CwAPI3D::Geometry::Vec3D& xDir,
                                                                     const CwAPI3D::Geometry::Vec3D& yDir,
                                                                     edgeprofile::EEdgeSymbolSize size,
                                                                     double minSize) const override;

private:
    [[nodiscard]] static double getBaseLength(edgeprofile::EEdgeSymbolSize size, double minSize);
};

}  // namespace borderline::adapter::cadwork
