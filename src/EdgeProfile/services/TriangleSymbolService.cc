#include "TriangleSymbolService.hh"

#include <algorithm>
#include <cmath>

#include "../CabinetAggregate.hh"
#include "../CabinetElement.hh"
#include "../utility/VectorAlignmentUtility.hh"
#include "EdgeGeometryUtils.hh"

namespace borderline::edgeprofile {

TriangleSymbolService::TriangleSymbolService(std::shared_ptr<ports::ITriangleStrategy> strategy,
                                             std::shared_ptr<ports::IElementControlPort> elementControlPort)
    : mStrategy(std::move(strategy)),
      mElementControlPort(std::move(elementControlPort)) {
}

std::unordered_map<uint64_t, std::vector<ElementID>> TriangleSymbolService::createSymbols(
    const CabinetCompositeNode& rootNode, const EEdgeSymbolSize size) const {
    const auto PanelPointsList = generateSymbols(rootNode, size);
    std::unordered_map<uint64_t, std::vector<ElementID>> createdElements;

    for (const auto& [cabinElement, points] : PanelPointsList) {
        // Create 3 lines for the triangle
        createdElements[cabinElement].push_back(mElementControlPort->createLineFromPoints(points.p1, points.p2));
        createdElements[cabinElement].push_back(mElementControlPort->createLineFromPoints(points.p2, points.p3));
        createdElements[cabinElement].push_back(mElementControlPort->createLineFromPoints(points.p3, points.p1));
    }

    return createdElements;
}

std::unordered_map<uint64_t, ports::TrianglePoints> TriangleSymbolService::generateSymbols(
    const CabinetCompositeNode& rootNode, const EEdgeSymbolSize size) const {
    std::unordered_map<uint64_t, ports::TrianglePoints> symbols;

    rootNode.traverse([this, size, &symbols](const CabinetComponent& component) {
        if (!component.hasData()) {  // prevent runtime_error
            return;
        }
        if (const auto& componentData = component.data(); std::holds_alternative<CabinetElement>(componentData)) {
            if (const auto symbolPoint = processElement(&component, size)) {
                const auto& element = std::get<CabinetElement>(componentData);
                const auto elementId = element.getElementID();
                const auto& geometry = element.getGeometry();
                const auto projectedTrianglePts = EdgeGeometryUtils::projectTriangleToPlane(
                    *symbolPoint, geometry.getCoordinateFrame(), geometry.getHeight() / 2.0);

                symbols.emplace(elementId, projectedTrianglePts);
            }
        }
    });

    // auto traverse = [&](auto&& self, const CabinetCompositeNode& node) -> void {
    //     for (const auto& child : node) {
    //         const auto* component = child.get();
    //
    //         if (std::holds_alternative<CabinetElement>(component->data())) {
    //             if (const auto symbolPoint = processElement(component, size)) {
    //                 symbols.push_back(*symbolPoint);
    //             }
    //         }
    //
    //         if (component->isComposite()) {
    //             if (const auto* composite = dynamic_cast<const CabinetCompositeNode*>(component)) {
    //                 self(self, *composite);
    //             }
    //         }
    //     }
    // };
    //
    // traverse(traverse, rootNode);
    return symbols;
}

std::optional<ports::TrianglePoints> TriangleSymbolService::processElement(const CabinetComponent* component,
                                                                           const EEdgeSymbolSize size) const {
    const auto& element = std::get<CabinetElement>(component->data());
    const auto& geometry = element.getGeometry();

    const auto masterVectors = extractMasterVectors(component);
    if (!masterVectors) {
        return std::nullopt;
    }

    const auto frame = geometry.getCoordinateFrame();

    const auto alignedVectors = utility::VectorAlignmentUtility::alignMasterVectorsOnReferencePlane(
        frame.xAxis(), frame.yAxis(), frame.zAxis(), masterVectors->zl, masterVectors->xl);

    if (!alignedVectors) {
        return std::nullopt;
    }

    const auto triangleX = alignedVectors->xl;
    const auto triangleY = alignedVectors->yl;
    const auto center = calculateCenter(geometry);
    const double minSize = std::min(geometry.getLength(), geometry.getWidth());

    return mStrategy->calculatePoints(center, triangleX, triangleY, size, minSize);
}

std::optional<TriangleSymbolService::MasterVectors> TriangleSymbolService::extractMasterVectors(
    const CabinetComponent* component) {
    const auto* parent = component->parent();
    if (!parent) {
        return std::nullopt;
    }

    if (!std::holds_alternative<CabinetAggregate>(parent->data())) {
        return std::nullopt;
    }

    const auto& aggregate = std::get<CabinetAggregate>(parent->data());
    return MasterVectors{aggregate.getXDir(), aggregate.getZDir()};
}

CwAPI3D::Geometry::Point3D TriangleSymbolService::calculateCenter(const CabinetElementGeometry& geometry) {
    // The axis origin and end points define the element's central axis
    // The midpoint of this axis is the element's center
    const auto p1 = geometry.getAxisOriginPt();
    const auto p2 = geometry.getAxisEndPt();

    return CwAPI3D::Geometry::Point3D((p1.x() + p2.x()) / 2.0, (p1.y() + p2.y()) / 2.0, (p1.z() + p2.z()) / 2.0);
}

}  // namespace borderline::edgeprofile
