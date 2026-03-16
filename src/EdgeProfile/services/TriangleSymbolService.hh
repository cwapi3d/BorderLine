#pragma once

#include <borderline/edge_profile/edge_profile_export.h>
#include <cwapi3d/geometry/CoordinateFrame3D.h>
#include <cwapi3d/geometry/Vec3D.h>

#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "../CabinetNode.hh"
#include "../ports/IElementControlPort.hh"
#include "../ports/ITriangleStrategy.hh"
#include "../types/types.hh"

namespace borderline::edgeprofile {

class EDGE_PROFILE_EXPORT TriangleSymbolService {
public:
    TriangleSymbolService(std::shared_ptr<ports::ITriangleStrategy> strategy,
                          std::shared_ptr<ports::IElementControlPort> elementControlPort);

    [[nodiscard]] std::unordered_map<uint64_t, ports::TrianglePoints> generateSymbols(
        const CabinetCompositeNode& rootNode, EEdgeSymbolSize size) const;

    [[nodiscard]] std::unordered_map<uint64_t, std::vector<ElementID>> createSymbols(
        const CabinetCompositeNode& rootNode, EEdgeSymbolSize size) const;

private:
    std::shared_ptr<ports::ITriangleStrategy> mStrategy;
    std::shared_ptr<ports::IElementControlPort> mElementControlPort;

    [[nodiscard]] std::optional<ports::TrianglePoints> processElement(const CabinetComponent* component,
                                                                      EEdgeSymbolSize size) const;

    struct MasterVectors {
        CwAPI3D::Geometry::Vec3D xl;
        CwAPI3D::Geometry::Vec3D zl;
    };

    [[nodiscard]] static std::optional<MasterVectors> extractMasterVectors(const CabinetComponent* component);

    [[nodiscard]] static CwAPI3D::Geometry::Point3D calculateCenter(const CabinetElementGeometry& geometry);
};

}  // namespace borderline::edgeprofile
