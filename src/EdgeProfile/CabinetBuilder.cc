#include "CabinetBuilder.hh"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>

#include "CabinetElementAttributes.hh"
#include "CabinetElementGeometry.hh"
#include "EdgeGeometryUtils.hh"
#include "EdgeOffsetConfig.hh"
#include "cwapi3d/geometry/Plane3D.h"

namespace borderline::edgeprofile {

CabinetBuilder::CabinetBuilder(std::shared_ptr<ports::IElementControlPort> elementControlPort,
                               std::shared_ptr<ports::IGeometryControlPort> geometryControlPort,
                               std::shared_ptr<ports::IAttributeControlPort> attributeControlPort,
                               std::shared_ptr<ports::IElementFilterStrategy> filterStrategy,
                               std::vector<ElementID> aggregateIds,
                               std::shared_ptr<ports::IEdgeOffsetStrategy> offsetStrategy,
                               EdgeOffsetConfig offsetConfig)
    : mElementControlPort(std::move(elementControlPort)),
      mGeometryControlPort(std::move(geometryControlPort)),
      mAttributeControlPort(std::move(attributeControlPort)),
      mFilterStrategy(std::move(filterStrategy)),
      mAggregateIds(std::move(aggregateIds)),
      mOffsetStrategy(std::move(offsetStrategy)),
      mOffsetConfig(std::move(offsetConfig)) {
}

std::unique_ptr<CabinetCompositeNode> CabinetBuilder::build(const ProfileConfig &config) {
    // Create the root node, wrapping the aggregate in CabinetData
    auto rootNode = std::make_unique<CabinetCompositeNode>("Root");

    auto cabinetParentComponents = buildCabinetParentComponents(rootNode.get());

    for (const auto &elementId : mAggregateIds) {
        const auto parentElementAttributeID = mAttributeControlPort->getUserAttribute(elementId, AttributeIdx{501});
        int parentElementId = 0;
        try {
            parentElementId = std::stoi(parentElementAttributeID);

        } catch (const std::out_of_range &outOfRange) {
            std::ignore = outOfRange;
            parentElementId = 0;
        }
        const auto &component = cabinetParentComponents.at(parentElementId);

            CabinetElementGeometry geometry(elementId, mGeometryControlPort);
            const CabinetElementAttributes attributes(mAttributeControlPort, elementId, std::nullopt);

            CabinetElement element(elementId, geometry, attributes);
            // panel
            auto elementNode = std::make_unique<CabinetCompositeNode>("Element", CabinetData(std::move(element)));
            elementNode->setParent(component.get());

            auto edges = EdgeGeometryUtils::createEdgeLines(geometry);
            const auto edgeProfileLogic = std::make_unique<EdgeProfileLogic>(mAttributeControlPort, elementId, config);
            edgeProfileLogic->determineEdgeLogic(edges);

            // Apply offsets based on determined edge types
            if (mOffsetStrategy) {
                mOffsetStrategy->applyOffsets(edges, geometry.getCoordinateFrame(), mOffsetConfig);
            }

            EdgeGeometryUtils::projectEdgesToPlane(edges, geometry.getCoordinateFrame(), geometry.getHeight() / 2.0);

            for (const auto &edgeLine : edges | std::views::values) {
                if (edgeLine.hasEdge()) {
                    auto leaf = std::make_unique<CabinetLeafNode>("Line", CabinetData(edgeLine));
                    leaf->setParent(elementNode.get());
                    elementNode->addChild(std::move(leaf));
                }
            }

            component->addChild(std::move(elementNode));
    }
    for (auto &component : cabinetParentComponents | std::views::values) {
        if (component) {
            rootNode->addChild(std::move(component));
        }
    }

    return rootNode;
}

std::unordered_map<std::string, std::unique_ptr<CabinetCompositeNode>> CabinetBuilder::buildNestingComponents() const {
    std::unordered_map<std::string, std::unique_ptr<CabinetCompositeNode>> result;

    for (const auto &element : mAggregateIds) {
        if (const auto nestingNr = mAttributeControlPort->getAssociatedNestingNumber(element);
            !result.contains(nestingNr)) {
            const auto nestingNumber = Number(static_cast<uint32_t>(std::stoi(nestingNr)));
            auto nestingComponent = NestingAggregate(nestingNumber);
            result[nestingNr] = std::make_unique<CabinetCompositeNode>("NestingVolume", CabinetData(nestingComponent));
        }
    }

    return result;
}

int CabinetBuilder::extractAndConvertUserAttributeValue(const ElementID element,
                                                        const AttributeIdx attributeIdx) const {
    const auto value = mAttributeControlPort->getUserAttribute(element, attributeIdx);

    int valueNr = 0;
    try {
        valueNr = std::stoi(value);
    } catch (const std::exception &) {
        valueNr = 0;
    }
    return valueNr;
}

std::unordered_map<int, std::unique_ptr<CabinetCompositeNode>> CabinetBuilder::buildCabinetParentComponents(
    CabinetCompositeNode *parent) const {
    std::unordered_map<int, std::unique_ptr<CabinetCompositeNode>> result;

    for (const auto &element : mAggregateIds) {
        // TODO inject reference attribute idx

        if (const int valueNr = extractAndConvertUserAttributeValue(element, AttributeIdx{501});
            !result.contains(valueNr)) {
            CabinetAggregate aggregate(mGeometryControlPort, ElementID(valueNr));
            result[valueNr] = std::make_unique<CabinetCompositeNode>("Aggregate", CabinetData(std::move(aggregate)));
        }
    }

    std::ranges::for_each(result | std::views::values, [parent](auto &component) { component->setParent(parent); });

    return result;
}
}  // namespace borderline::edgeprofile
