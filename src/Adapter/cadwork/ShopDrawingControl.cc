//
// Created by MichaelBrunner on 12/12/2025.
//

#include "ShopDrawingControl.hh"

#include <cwapi3d/ICwAPI3DControllerFactory.h>
#include <cwapi3d/ICwAPI3DShopDrawingController.h>
#include <cwapi3d/utility/StringConversion.h>

namespace borderline::adapter::cadwork {

ShopDrawingControl::ShopDrawingControl(CwAPI3D::Interfaces::ICwAPI3DControllerFactory* factory)
    : mShopDrawingController((factory->getShopDrawingController())), mFactory(factory) {}

void ShopDrawingControl::loadExportPieceByPieceSettings(std::string aFilePath) {
  mShopDrawingController->loadExportPieceByPieceSettings(CwAPI3D::Utility::ToWString(std::move(aFilePath)).data());
}

void ShopDrawingControl::exportPieceByPieceWithClipboard(edgeprofile::Number clipBoardIndex,
                                                         const std::vector<edgeprofile::ElementID>& aElements) {
  const auto elementIdList = mFactory->createEmptyElementIDList();
  for (const auto& elementID : aElements) {
    elementIdList->append(elementID.get());
  }
  mShopDrawingController->exportPieceByPieceWithClipboard(clipBoardIndex.get(), elementIdList);
}
}  // namespace borderline::adapter::cadwork