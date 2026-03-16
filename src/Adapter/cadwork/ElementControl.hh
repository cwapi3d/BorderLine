//
// Created by MichaelBrunner on 08/12/2025.
//

#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include "IElementControlPort.hh"

namespace CwAPI3D::Interfaces {
class ICwAPI3DControllerFactory;
class ICwAPI3DElementController;
}  // namespace CwAPI3D::Interfaces

namespace borderline::adapter::cadwork {
class CADWORK_ADAPTER_EXPORT ElementFilters : public edgeprofile::ports::IElementFilterStrategy {
public:
    ElementFilters(const ElementFilters &aOther) = default;
    ElementFilters(ElementFilters &&aOther) noexcept = default;
    ElementFilters &operator=(const ElementFilters &aOther) = default;
    ElementFilters &operator=(ElementFilters &&aOther) noexcept = default;
    ~ElementFilters() override = default;

    explicit ElementFilters(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory);

    [[nodiscard]] bool isPanel(edgeprofile::ElementID id) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory *mFactory{nullptr};
};

class CADWORK_ADAPTER_EXPORT ElementControl : public edgeprofile::ports::IElementControlPort {
public:
    explicit ElementControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory);

    ElementControl(const ElementControl &aOther) = default;
    ElementControl(ElementControl &&aOther) noexcept = default;
    auto operator=(const ElementControl &aOther) -> ElementControl & = default;
    auto operator=(ElementControl &&aOther) noexcept -> ElementControl & = default;

    ~ElementControl() override = default;

    [[nodiscard]] auto getActiveElementIDs() const -> std::vector<edgeprofile::ElementID> override;

    void recreateElements(const std::vector<edgeprofile::ElementID> &elementIDs) const override;

    [[nodiscard]] auto createLineFromPoints(const CwAPI3D::Geometry::Point3D &point1,
                                            const CwAPI3D::Geometry::Point3D &point2) const
        -> edgeprofile::ElementID override;

    [[nodiscard]] auto getNestingParentID(edgeprofile::ElementID element) const
        -> edgeprofile::NestingParentID override;

    [[nodiscard]] auto getNestingElementID(edgeprofile::ElementID elementID) -> edgeprofile::ElementID override;

    [[nodiscard]] auto getActiveElementIDsFilter(const FilterFunction &filter) const
        -> std::vector<edgeprofile::ElementID> override;
    [[nodiscard]] std::vector<CwAPI3D::Geometry::Point3D> getBoundingBoxVerticesLocal(
        edgeprofile::ElementID elementID) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory *mFactory{nullptr};
    CwAPI3D::Interfaces::ICwAPI3DElementController *mElementController{nullptr};
};
}  // namespace borderline::adapter::cadwork
