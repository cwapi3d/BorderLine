//
// Created by MichaelBrunner on 15/12/2025.
//

#include "VisualizationControl.hh"

#include <cwapi3d/ICwAPI3DControllerFactory.h>
#include <cwapi3d/ICwAPI3DVisualizationController.h>

namespace borderline::adapter::cadwork {

VisualizationControl::VisualizationControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* aFactory)
    : mFactory(aFactory),
      mVisualizationController(aFactory->getVisualizationController()) {
}

void VisualizationControl::setColor(const std::vector<edgeprofile::ElementID>& elementIDs,
                                    edgeprofile::Number colorNumber) {
    auto* const elementIdList = mFactory->createEmptyElementIDList();
    for (const auto& elementID : elementIDs) {
        elementIdList->append(elementID.get());
    }

    mVisualizationController->setColor(elementIdList, colorNumber.get());
}
edgeprofile::ports::IVisualizationControlPort::RGB VisualizationControl::getRgbColor(
    edgeprofile::Number colorNumber) const {
    const auto [mR, mG, mB] = mVisualizationController->getRGBFromCadworkColorID(colorNumber.get());
    return RGB{.r = static_cast<uint8_t>(mR), .g = static_cast<uint8_t>(mG), .b = static_cast<uint8_t>(mB)};
}
}  // namespace borderline::adapter::cadwork