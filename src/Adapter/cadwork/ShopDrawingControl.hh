//
// Created by MichaelBrunner on 12/12/2025.
//

#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include "IShopDrawingControlPort.hh"

namespace CwAPI3D::Interfaces {
class ICwAPI3DControllerFactory;
class ICwAPI3DShopDrawingController;
}
namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT ShopDrawingControl : public edgeprofile::ports::IShopDrawingControlPort {
 public:
  explicit ShopDrawingControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* factory);

  ~ShopDrawingControl() override = default;

  void loadExportPieceByPieceSettings(std::string aFilePath) override;

  void exportPieceByPieceWithClipboard(edgeprofile::Number clipBoardIndex,
                                       const std::vector<edgeprofile::ElementID> &aElements) override;

 private:
  CwAPI3D::Interfaces::ICwAPI3DShopDrawingController *mShopDrawingController{nullptr};
  CwAPI3D::Interfaces::ICwAPI3DControllerFactory* mFactory{nullptr};
};

}  // namespace borderline::adapter::cadwork