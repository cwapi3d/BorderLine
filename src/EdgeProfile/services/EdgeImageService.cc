//
// Created by MichaelBrunner on 09/12/2025.
//

#include "EdgeImageService.hh"

#include "AttributeMappingService.hh"
#include "CabinetBuilder.hh"
#include "CabinetNode.hh"

namespace borderline::edgeprofile {
EdgeImageService::EdgeImageService(std::shared_ptr<ports::IElementControlPort> elementControlPort,
                                   std::shared_ptr<ports::IGeometryControlPort> geometryControlPort,
                                   std::shared_ptr<ports::IAttributeControlPort> attributeControlPort,
                                   std::shared_ptr<ports::IElementFilterStrategy> &&filterStrategy,
                                   std::shared_ptr<TriangleSymbolService> triangleSymbolService,
                                   std::shared_ptr<ports::IEdgeOffsetStrategy> offsetStrategy,
                                   EdgeOffsetConfig offsetConfig)
    : mElementControlPort(std::move(elementControlPort)),
      mGeometryControlPort(std::move(geometryControlPort)),
      mAttributeControlPort(std::move(attributeControlPort)),
      mFilterStrategy(std::move(filterStrategy)),
      mTriangleSymbolService(std::move(triangleSymbolService)),
      mOffsetStrategy(std::move(offsetStrategy)),
      mOffsetConfig(std::move(offsetConfig)) {
}

ports::ElementGroupings EdgeImageService::createEdgeImageLinesAndGroupCabinElementsByNestingParent(
    const ProfileConfig &config, const std::vector<ElementID> &aElementSelection) {
    CabinetBuilder builder(mElementControlPort, mGeometryControlPort, mAttributeControlPort, mFilterStrategy,
                           aElementSelection, mOffsetStrategy, mOffsetConfig);

    const auto cabinetCompositeNode = builder.build(config);

    std::unordered_map<uint64_t, std::vector<ElementID>> cabinElementSymbolLines;

    // Generate triangle symbols
    if (config.mCreateSymbol) {
        const auto symbolIds = mTriangleSymbolService->createSymbols(*cabinetCompositeNode, config.mSymbolSize);
        // cabinElementSymbolLines.insert(symbolIds.begin(), symbolIds.end());
        for (const auto &[key, value] : symbolIds) {
            cabinElementSymbolLines[key].insert(cabinElementSymbolLines[key].end(), value.begin(), value.end());
        }
    }

    // Generate edge lines
    if (config.mCreateEdges) {
        const auto lineIds = generateEdgeLines(*cabinetCompositeNode);
        for (const auto &[key, value] : lineIds) {
            cabinElementSymbolLines[key].insert(cabinElementSymbolLines[key].end(), value.begin(), value.end());
        }
    }

    auto panelsSortedByNestingData = generateOrderedByAssociatedNestingVolume(*cabinetCompositeNode);
    // return orderedData;

    return ports::ElementGroupings{panelsSortedByNestingData, cabinElementSymbolLines};
}

std::unordered_map<std::string, std::vector<ElementID>> EdgeImageService::generateOrderedByAssociatedNestingVolume(
    const CabinetCompositeNode &compositeNode) const {
    std::unordered_map<std::string, std::vector<ElementID>> elementsByNestingAssociation;

    compositeNode.traverse([this, &elementsByNestingAssociation](const CabinetComponent &component) {
        if (!component.hasData()) {  // prevent runtime_error
            return;
        }
        if (std::holds_alternative<CabinetElement>(component.data())) {
            const auto &cabinetElement = std::get<CabinetElement>(component.data());
            const auto nestingNumber = mAttributeControlPort->getAssociatedNestingNumber(cabinetElement.getElementID());
            elementsByNestingAssociation[nestingNumber].push_back(cabinetElement.getElementID());
        }
    });

    return elementsByNestingAssociation;
}

std::unordered_map<uint64_t, std::vector<ElementID>> EdgeImageService::processPanel(CabinetBuilder &builder,
                                                                                    const ProfileConfig &config) const {
    const auto rootNode = builder.build(config);

    std::unordered_map<uint64_t, std::vector<ElementID>> panelIds;

    // Generate triangle symbols
    if (config.mCreateSymbol) {
        const auto symbolIds = mTriangleSymbolService->createSymbols(*rootNode, config.mSymbolSize);
        // panelIds.insert(symbolIds.begin(), symbolIds.end());
        for (const auto &[key, value] : symbolIds) {
            panelIds[key].insert(panelIds[key].end(), value.begin(), value.end());
        }
    }

    // Generate edge lines
    if (config.mCreateEdges) {
        const auto lineIds = generateEdgeLines(*rootNode);
        for (const auto &[key, value] : lineIds) {
            panelIds[key].insert(panelIds[key].end(), value.begin(), value.end());
        }
    }

    return panelIds;
}

std::unordered_map<uint64_t, std::vector<ElementID>> EdgeImageService::generateEdgeLines(
    CabinetCompositeNode &rootNode) const {
    std::unordered_map<uint64_t, std::vector<ElementID>> lineIds;

    rootNode.traverse([this, &lineIds](CabinetComponent &component) {
        if (!component.hasData()) {  // prevent runtime_error
            return;
        }
        if (std::holds_alternative<EdgeLine>(component.data())) {
            if (auto &edgeLine = std::get<EdgeLine>(component.data()); edgeLine.hasEdge()) {
                const auto parent = component.parent();
                if (!parent || !parent->isComposite() || !parent->hasData()) {
                    return;
                }
                if (std::holds_alternative<CabinetElement>(parent->data())) {
                    const auto id = mElementControlPort->createLineFromPoints(edgeLine.getStartPoint().getPosition(),
                                                                              edgeLine.getEndPoint().getPosition());
                    edgeLine.setLineElementId(id);
                    // lineIds.emplace(std::get<CabinetElement>(parent->data()).getElementID(), id);
                    lineIds[std::get<CabinetElement>(parent->data()).getElementID().get()].push_back(id);
                }
            }
        }
    });

    return lineIds;
}
}  // namespace borderline::edgeprofile
