#pragma once

#include <borderline/edge_profile/edge_profile_export.h>
#include <memory>
#include <vector>
#include <map>
#include "CabinetNode.hh"
#include "ports/IElementControlPort.hh"
#include "ports/IGeometryControlPort.hh"
#include "ports/IAttributeControlPort.hh"
#include "ports/IEdgeOffsetStrategy.hh"
#include "EdgeProfileLogic.hh"
#include "EdgeOffsetConfig.hh"

namespace borderline::edgeprofile
{
class EDGE_PROFILE_EXPORT CabinetBuilder
{
public:
    CabinetBuilder(std::shared_ptr<ports::IElementControlPort> elementControlPort,
                   std::shared_ptr<ports::IGeometryControlPort> geometryControlPort,
                   std::shared_ptr<ports::IAttributeControlPort> attributeControlPort,
                   std::shared_ptr<ports::IElementFilterStrategy> filterStrategy,
                   std::vector<ElementID> aggregateIds,
                   std::shared_ptr<ports::IEdgeOffsetStrategy> offsetStrategy,
                   EdgeOffsetConfig offsetConfig = EdgeOffsetConfig{});

    std::unique_ptr<CabinetCompositeNode> build(const ProfileConfig& config);

private:
    std::shared_ptr<ports::IElementControlPort> mElementControlPort;
    std::shared_ptr<ports::IGeometryControlPort> mGeometryControlPort;
    std::shared_ptr<ports::IAttributeControlPort> mAttributeControlPort;
    std::shared_ptr<ports::IElementFilterStrategy> mFilterStrategy;
    std::vector<ElementID> mAggregateIds;
    std::shared_ptr<ports::IEdgeOffsetStrategy> mOffsetStrategy;
    EdgeOffsetConfig mOffsetConfig;

    [[nodiscard]] std::unordered_map<std::string, std::unique_ptr<CabinetCompositeNode>> buildNestingComponents() const;
    int extractAndConvertUserAttributeValue(ElementID element, AttributeIdx attributeIdx) const;
    [[nodiscard]] std::unordered_map<int, std::unique_ptr<CabinetCompositeNode>> buildCabinetParentComponents(
        CabinetCompositeNode* parent) const;
};
}
