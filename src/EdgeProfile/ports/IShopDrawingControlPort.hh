//
// Created by MichaelBrunner on 12/12/2025.
//

#pragma once
#include <string>
#include <vector>

#include "types/types.hh"

namespace borderline::edgeprofile::ports {
class IShopDrawingControlPort {
 public:
  virtual ~IShopDrawingControlPort() = default;

  virtual void loadExportPieceByPieceSettings(std::string aFilePath) = 0;

  virtual void exportPieceByPieceWithClipboard(Number clipBoardIndex, const std::vector<ElementID>& aElements) = 0;
};
}  // namespace borderline::edgeprofile::ports