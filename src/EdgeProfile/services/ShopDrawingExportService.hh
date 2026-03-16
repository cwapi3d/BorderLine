#pragma once

#include <vector>
#include <string>
#include <functional>
#include <atomic>
#include <memory>
#include <borderline/edge_profile/edge_profile_export.h>

#include "types/types.hh"
#include "IShopDrawingControlPort.hh"
#include "IAttributeControlPort.hh"
#include "ISortingStrategy.hh"

namespace borderline::edgeprofile::services {

class EDGE_PROFILE_EXPORT ShopDrawingExportService {
public:
    ShopDrawingExportService(std::shared_ptr<ports::IShopDrawingControlPort> shopDrawingPort,
                             std::shared_ptr<ports::IAttributeControlPort> attributePort);

    void loadSettings(const std::string& iniPath) const;
    void exportGroup(const std::vector<ElementID>& group, int clipboardIndex) const;

private:
    std::shared_ptr<ports::IShopDrawingControlPort> mShopDrawingPort;
    std::shared_ptr<ports::IAttributeControlPort> mAttributePort;
};

}
