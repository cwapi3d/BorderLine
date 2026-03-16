//
// Created by MichaelBrunner on 09/12/2025.
//

#include "AttributeControl.hh"

#include <cwapi3d/ICwAPI3DAttributeController.h>
#include <cwapi3d/ICwAPI3DControllerFactory.h>
#include <cwapi3d/utility/StringConversion.h>

#include <iostream>
#include <ostream>

namespace borderline::adapter::cadwork {
AttributeControl::AttributeControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory *factory)
    : mFactory(factory),
      mAttributeController(factory->getAttributeController()) {
}

auto AttributeControl::getUserAttribute(edgeprofile::ElementID elementID, edgeprofile::AttributeIdx attributeIdx) const
    -> std::string {
    auto *const value = mAttributeController->getUserAttribute(elementID.get(), attributeIdx.get());
    return CwAPI3D::Utility::ToUtf8(value->data());
}
edgeprofile::Number AttributeControl::getProductionNumber(edgeprofile::ElementID elementID) const {
    return edgeprofile::Number{mAttributeController->getProductionNumber(elementID.get())};
}
edgeprofile::Number AttributeControl::getAssemblyNumber(edgeprofile::ElementID elementID) const {
    auto *assemblyId = mAttributeController->getAssemblyNumber(elementID.get());
    const auto assemblyIdStdStr = CwAPI3D::Utility::ToUtf8(assemblyId->data());
    try {
        return edgeprofile::Number{static_cast<uint32_t>(std::stoi(assemblyIdStdStr))};
    } catch (const std::invalid_argument &invalidArg) {
        std::cerr << "Invalid assembly number: " << assemblyIdStdStr << invalidArg.what() << std::endl;
        return edgeprofile::Number{0};
    }
}
void AttributeControl::setProcessType(edgeprofile::ElementID elementID,
                                      const edgeprofile::ports::ProcessType processType) const {
    auto *const elementList = mFactory->createElementIDListFromElement(elementID.get());

    auto *const processTypeObj = mFactory->createEmptyProcessType();
    if (processType == edgeprofile::ports::ProcessType::Panel_1) {
        processTypeObj->setPanel1();
    }

    mAttributeController->setOutputType(elementList, processTypeObj);
}
void AttributeControl::setProductionNumber(edgeprofile::ElementID elementID,
                                           edgeprofile::Number productionNumber) const {
    auto *const elementList = mFactory->createElementIDListFromElement(elementID.get());
    mAttributeController->setProductionNumber(elementList, productionNumber.get());
}
void AttributeControl::setAssemblyNumber(edgeprofile::ElementID elementID, edgeprofile::Number assemblyNumber) const {
    auto *const elementList = mFactory->createElementIDListFromElement(elementID.get());
    mAttributeController->setAssemblyNumber(elementList,
                                            CwAPI3D::Utility::ToWString(std::to_string(assemblyNumber.get())).data());
}

std::string AttributeControl::getAssociatedNestingNumber(edgeprofile::ElementID elementID) const {
    const auto data = mAttributeController->getAssociatedNestingNumber(elementID.get());
    return CwAPI3D::Utility::ToUtf8(data->data());
}

std::string AttributeControl::getSubgroup(edgeprofile::ElementID elementID) const {
    const auto data = mAttributeController->getSubgroup(elementID.get());
    return CwAPI3D::Utility::ToUtf8(data->data());
}

void AttributeControl::setSubgroup(edgeprofile::ElementID elementID, const std::string subgroup) const {
    mAttributeController->setSubgroup(mFactory->createElementIDListFromElement(elementID.get()),
                                      CwAPI3D::Utility::ToWString(subgroup).data());
}

std::string AttributeControl::getGroup(edgeprofile::ElementID elementID) const {
    const auto data = mAttributeController->getGroup(elementID.get());
    return CwAPI3D::Utility::ToUtf8(data->data());
}

void AttributeControl::setGroup(edgeprofile::ElementID elementID, const std::string group) const {
    mAttributeController->setGroup(mFactory->createElementIDListFromElement(elementID.get()),
                                   CwAPI3D::Utility::ToWString(group).data());
}
}  // namespace borderline::adapter::cadwork
