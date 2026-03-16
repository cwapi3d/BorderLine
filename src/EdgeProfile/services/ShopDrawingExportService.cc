#include "ShopDrawingExportService.hh"
#include <thread>
#include <chrono>

namespace borderline::edgeprofile::services {

ShopDrawingExportService::ShopDrawingExportService(std::shared_ptr<ports::IShopDrawingControlPort> shopDrawingPort,
                                                   std::shared_ptr<ports::IAttributeControlPort> attributePort)
    : mShopDrawingPort(std::move(shopDrawingPort)),
      mAttributePort(std::move(attributePort))
{
}

void ShopDrawingExportService::loadSettings(const std::string& iniPath) const {
    mShopDrawingPort->loadExportPieceByPieceSettings(iniPath);
}

void ShopDrawingExportService::exportGroup(const std::vector<ElementID>& group, const int clipboardIndex) const {
    if (!group.empty()) {
        mShopDrawingPort->exportPieceByPieceWithClipboard(Number{static_cast<uint32_t>(clipboardIndex)}, group);
    }
}

}
