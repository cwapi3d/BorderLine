#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include <ports/IEdgeOffsetStrategy.hh>

namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT StandardEdgeOffsetStrategy final : public edgeprofile::ports::IEdgeOffsetStrategy {
public:
    void applyOffsets(std::map<edgeprofile::EEdgePosition, edgeprofile::EdgeLine>& edges,
                      const CwAPI3D::Geometry::CoordinateFrame3D& coordFrame,
                      const edgeprofile::EdgeOffsetConfig& offsetConfig) const override;
};

}  // namespace borderline::adapter::cadwork
