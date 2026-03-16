//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include <IAttributeControlPort.hh>

namespace CwAPI3D::Interfaces {
class ICwAPI3DControllerFactory;
class ICwAPI3DAttributeController;
}  // namespace CwAPI3D::Interfaces

namespace borderline::adapter::cadwork {
class CADWORK_ADAPTER_EXPORT AttributeControl : public edgeprofile::ports::IAttributeControlPort {
public:
    AttributeControl(const AttributeControl &aOther) = default;
    AttributeControl(AttributeControl &&aOther) noexcept = default;
    AttributeControl &operator=(const AttributeControl &aOther) = default;
    AttributeControl &operator=(AttributeControl &&aOther) noexcept = default;

    ~AttributeControl() override = default;
    explicit AttributeControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory);

    [[nodiscard]] auto getUserAttribute(edgeprofile::ElementID elementID, edgeprofile::AttributeIdx attributeIdx) const
        -> std::string override;
    [[nodiscard]] edgeprofile::Number getProductionNumber(edgeprofile::ElementID elementID) const override;
    [[nodiscard]] edgeprofile::Number getAssemblyNumber(edgeprofile::ElementID elementID) const override;
    void setProcessType(edgeprofile::ElementID elementID, edgeprofile::ports::ProcessType processType) const override;
    void setProductionNumber(edgeprofile::ElementID elementID, edgeprofile::Number productionNumber) const override;
    void setAssemblyNumber(edgeprofile::ElementID elementID, edgeprofile::Number assemblyNumber) const override;
    std::string getAssociatedNestingNumber(edgeprofile::ElementID elementID) const override;

    [[nodiscard]] std::string getSubgroup(edgeprofile::ElementID elementID) const override;
    void setSubgroup(edgeprofile::ElementID elementID, std::string subgroup) const override;

    [[nodiscard]] std::string getGroup(edgeprofile::ElementID elementID) const override;
    void setGroup(edgeprofile::ElementID elementID, std::string group) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory *mFactory{nullptr};
    CwAPI3D::Interfaces::ICwAPI3DAttributeController *mAttributeController{nullptr};
};
}  // namespace borderline::adapter::cadwork
