#pragma once

#include <borderline/edge_profile/edge_profile_export.h>

#include <memory>
#include <vector>

#include "ports/IUtilityControlPort.hh"
#include "ports/IVisualizationControlPort.hh"
#include "types/types.hh"

namespace borderline::edgeprofile::services {

class EDGE_PROFILE_EXPORT ColorApplicationService final {
public:
    explicit ColorApplicationService(std::shared_ptr<ports::IUtilityControlPort> utilityControlPort,
                                     std::shared_ptr<ports::IVisualizationControlPort> visualizationControlPort);

    void applyColorToElements(Number colorNumber, const std::vector<ElementID>& elementIds) const;

private:
    std::shared_ptr<ports::IUtilityControlPort> mUtilityControlPort;
    std::shared_ptr<ports::IVisualizationControlPort> mVisualizationControlPort;

    [[nodiscard]] uint32_t convertColorNumber(Number colorNumber) const;
};

}  // namespace borderline::edgeprofile::services
