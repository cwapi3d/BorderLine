#include "ColorApplicationService.hh"

namespace borderline::edgeprofile::services {

ColorApplicationService::ColorApplicationService(
    std::shared_ptr<ports::IUtilityControlPort> utilityControlPort,
    std::shared_ptr<ports::IVisualizationControlPort> visualizationControlPort)
    : mUtilityControlPort(std::move(utilityControlPort)),
      mVisualizationControlPort(std::move(visualizationControlPort)) {
}

void ColorApplicationService::applyColorToElements(const Number colorNumber,
                                                   const std::vector<ElementID>& elementIds) const {
    if (elementIds.empty()) {
        return;
    }

    mVisualizationControlPort->setColor(elementIds, colorNumber);
}

uint32_t ColorApplicationService::convertColorNumber(const Number colorNumber) const {
    return mUtilityControlPort->getUserColor(colorNumber);
}

}  // namespace borderline::edgeprofile::services
