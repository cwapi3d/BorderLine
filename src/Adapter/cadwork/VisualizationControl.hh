//
// Created by MichaelBrunner on 15/12/2025.
//

#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include "IVisualizationControlPort.hh"

namespace CwAPI3D::Interfaces {
class ICwAPI3DVisualizationController;
class ICwAPI3DControllerFactory;
}  // namespace CwAPI3D::Interfaces

namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT VisualizationControl : public edgeprofile::ports::IVisualizationControlPort {
public:
    VisualizationControl(const VisualizationControl& aOther) = default;
    VisualizationControl(VisualizationControl&& aOther) noexcept = default;
    VisualizationControl& operator=(const VisualizationControl& aOther) = default;
    VisualizationControl& operator=(VisualizationControl&& aOther) noexcept = default;
    explicit VisualizationControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* aFactory);

    ~VisualizationControl() override = default;

    void setColor(const std::vector<edgeprofile::ElementID>& elementIDs, edgeprofile::Number colorNumber) override;
    [[nodiscard]] RGB getRgbColor(edgeprofile::Number colorNumber) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory* mFactory{nullptr};
    CwAPI3D::Interfaces::ICwAPI3DVisualizationController* mVisualizationController{nullptr};
};

}  // namespace borderline::adapter::cadwork