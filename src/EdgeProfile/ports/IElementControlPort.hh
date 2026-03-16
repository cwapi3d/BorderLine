//
// Created by MichaelBrunner on 08/12/2025.
//

#pragma once
#include <cstdint>
#include <functional>
#include <vector>

#include "cwapi3d/geometry/BoundingBox.h"
#include "types/types.hh"

namespace borderline::edgeprofile::ports
{
class IElementFilterStrategy
{
public:
    virtual ~IElementFilterStrategy() = default;

    virtual bool isPanel(ElementID id) const = 0;

   // virtual bool operator()(ElementID id) const { return isPanel(id); }
};

class IElementControlPort
{
public:
    virtual ~IElementControlPort() = default;

    [[nodiscard]] virtual auto getActiveElementIDs() const -> std::vector<ElementID> = 0;

    using FilterFunction = std::function<bool(ElementID)>;
    [[nodiscard]] virtual auto getActiveElementIDsFilter(const FilterFunction &filter) const -> std::vector<ElementID> =
    0;

    virtual void recreateElements(const std::vector<ElementID> &elementIDs) const = 0;

    [[nodiscard]] virtual auto createLineFromPoints(const CwAPI3D::Geometry::Point3D &point1,
                                                    const CwAPI3D::Geometry::Point3D &point2) const -> ElementID = 0;

    [[nodiscard]] virtual auto getNestingParentID(ElementID element) const -> NestingParentID = 0;
    [[nodiscard]] virtual auto getNestingElementID(ElementID elementID) -> ElementID = 0;


    [[nodiscard]] virtual std::vector<CwAPI3D::Geometry::Point3D> getBoundingBoxVerticesLocal(ElementID elementID) const
    = 0;
};
}
