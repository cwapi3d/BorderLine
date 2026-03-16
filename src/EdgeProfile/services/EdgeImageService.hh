//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include <borderline/edge_profile/edge_profile_export.h>

#include <memory>

#include "../CabinetBuilder.hh"
#include "../EdgeOffsetConfig.hh"
#include "TriangleSymbolService.hh"
#include "ports/IAttributeControlPort.hh"
#include "ports/IEdgeImageGenerator.hh"
#include "ports/IEdgeOffsetStrategy.hh"
#include "ports/IElementControlPort.hh"
#include "ports/IGeometryControlPort.hh"

namespace borderline::edgeprofile {
namespace services {
class AttributeMappingService;
}

class EDGE_PROFILE_EXPORT EdgeImageService : public ports::IEdgeImageGenerator {
public:
    EdgeImageService(std::shared_ptr<ports::IElementControlPort> elementControlPort,
                     std::shared_ptr<ports::IGeometryControlPort> geometryControlPort,
                     std::shared_ptr<ports::IAttributeControlPort> attributeControlPort,
                     std::shared_ptr<ports::IElementFilterStrategy>&& filterStrategy,
                     std::shared_ptr<TriangleSymbolService> triangleSymbolService,
                     std::shared_ptr<ports::IEdgeOffsetStrategy> offsetStrategy,
                     EdgeOffsetConfig offsetConfig = EdgeOffsetConfig{});

    ports::ElementGroupings createEdgeImageLinesAndGroupCabinElementsByNestingParent(
        const ProfileConfig& config, const std::vector<ElementID>& aElementSelection) override;

private:
    [[nodiscard]] std::unordered_map<uint64_t, std::vector<ElementID>> processPanel(CabinetBuilder& builder,
                                                                                    const ProfileConfig& config) const;
    [[nodiscard]] std::unordered_map<uint64_t, std::vector<ElementID>> generateEdgeLines(
        CabinetCompositeNode& rootNode) const;
    [[nodiscard]] std::unordered_map<std::string, std::vector<ElementID>> generateOrderedByAssociatedNestingVolume(
        const CabinetCompositeNode& compositeNode) const;

    std::shared_ptr<ports::IElementControlPort> mElementControlPort;
    std::shared_ptr<ports::IGeometryControlPort> mGeometryControlPort;
    std::shared_ptr<ports::IAttributeControlPort> mAttributeControlPort;
    std::shared_ptr<ports::IElementFilterStrategy> mFilterStrategy;
    std::shared_ptr<TriangleSymbolService> mTriangleSymbolService;
    std::shared_ptr<ports::IEdgeOffsetStrategy> mOffsetStrategy;
    EdgeOffsetConfig mOffsetConfig;
};
}  // namespace borderline::edgeprofile
