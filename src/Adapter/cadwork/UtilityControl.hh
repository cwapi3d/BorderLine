#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include "ports/IUtilityControlPort.hh"

namespace CwAPI3D::Interfaces {
class ICwAPI3DControllerFactory;
}
namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT UtilityControl final : public edgeprofile::ports::IUtilityControlPort {
public:
    explicit UtilityControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* factory);

    [[nodiscard]] uint32_t getUserColor(edgeprofile::Number colorNumber) const override;

private:
    CwAPI3D::Interfaces::ICwAPI3DControllerFactory* mFactory;
};

}  // namespace borderline::adapter::cadwork
