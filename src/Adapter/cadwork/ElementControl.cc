//
// Created by MichaelBrunner on 08/12/2025.
//

#include "ElementControl.hh"

#include <cwapi3d/CwAPI3D.h>
#include <cwapi3d/geometry/BoundingBox.h>

#include <ranges>

namespace {
auto extractElementIDs(CwAPI3D::Interfaces::ICwAPI3DElementIDList *const elementIdList)
    -> std::vector<borderline::edgeprofile::ElementID> {
    std::vector<borderline::edgeprofile::ElementID> elements;
    for (decltype(elementIdList->count()) i = 0; std::cmp_less(i, elementIdList->count()); ++i) {
        elements.emplace_back(elementIdList->at(i));
    }
    return elements;
}
}  // namespace

namespace borderline::adapter::cadwork {
ElementFilters::ElementFilters(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory)
    : mFactory(factory) {
}

bool ElementFilters::isPanel(edgeprofile::ElementID id) const {
    return mFactory->getAttributeController()->getElementType(id.get())->isPanel();
}

ElementControl::ElementControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory)
    : mFactory(factory),
      mElementController(factory->getElementController()) {
}

auto ElementControl::getActiveElementIDs() const -> std::vector<edgeprofile::ElementID> {
    auto *const elementIdList = mElementController->getActiveIdentifiableElementIDs();
    std::vector<edgeprofile::ElementID> elements = extractElementIDs(elementIdList);

    return elements;
}

void ElementControl::recreateElements(const std::vector<edgeprofile::ElementID> &elementIDs) const {
    auto *const elementIdList = mFactory->createEmptyElementIDList();

    for (const auto &elementID : elementIDs) {
        elementIdList->append(elementID.get());
    }

    mElementController->recreateElements(elementIdList);
}

auto ElementControl::createLineFromPoints(const CwAPI3D::Geometry::Point3D &point1,
                                          const CwAPI3D::Geometry::Point3D &point2) const -> edgeprofile::ElementID {
    const auto lineId = mElementController->createLinePoints({.mX = point1.x(), .mY = point1.y(), .mZ = point1.z()},
                                                             {.mX = point2.x(), .mY = point2.y(), .mZ = point2.z()});
    return edgeprofile::ElementID(lineId);
}

auto ElementControl::getNestingParentID(edgeprofile::ElementID element) const -> edgeprofile::NestingParentID {
    const auto nestingParentId = mElementController->getNestingParentId(element.get());
    return edgeprofile::NestingParentID(nestingParentId);
}

auto ElementControl::getNestingElementID(edgeprofile::ElementID elementID) -> edgeprofile::ElementID {
    const auto nestingElementId = mElementController->getNestingParentId(elementID.get());
    return edgeprofile::ElementID(nestingElementId);
}

auto ElementControl::getActiveElementIDsFilter(const FilterFunction &filter) const
    -> std::vector<edgeprofile::ElementID> {
    const auto elements = getActiveElementIDs();

    return elements | std::views::filter(filter) | std::ranges::to<std::vector<edgeprofile::ElementID> >();
    // std::vector<edgeprofile::ElementID> filteredElements;
    // std::ranges::transform(elements | std::views::filter(filter), std::back_inserter(filteredElements),
    //                        [](const auto elementId) { return edgeprofile::ElementID(elementId); });
    //
    // return filteredElements;
}

std::vector<CwAPI3D::Geometry::Point3D> ElementControl::getBoundingBoxVerticesLocal(
    edgeprofile::ElementID elementID) const {
    auto *const points = mElementController->getBoundingBoxVerticesLocal(
        elementID.get(), mFactory->createElementIDListFromElement(elementID.get()));

    std::vector<CwAPI3D::Geometry::Point3D> obbPoints;
    obbPoints.reserve(points->count());
    for (decltype(points->count()) i = 0; std::cmp_less(i, points->count()); ++i) {
        obbPoints.emplace_back(points->at(i).mX, points->at(i).mY, points->at(i).mZ);
    }
    return obbPoints;
}
}  // namespace borderline::adapter::cadwork
// adapter
// borderline
