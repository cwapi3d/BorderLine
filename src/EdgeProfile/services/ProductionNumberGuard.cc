#include "ProductionNumberGuard.hh"

#include <algorithm>

namespace borderline::edgeprofile::services {

ProductionNumberGuard::ProductionNumberGuard(std::shared_ptr<ports::IAttributeControlPort> attributePort,
                                             const std::vector<ElementID>& elements, const uint32_t temporaryNumberStart)
    : mAttributePort(std::move(attributePort))
    , mElements(elements)
    , mTemporaryNumberStart(temporaryNumberStart) {
  saveAndAssignTemporaryNumbers();
}

ProductionNumberGuard::~ProductionNumberGuard() {
  try {
    restoreOriginalNumbers();
  } catch (...) {
    // Suppress exceptions in destructor to maintain RAII safety
    // In production, log the error appropriately
  }
}

void ProductionNumberGuard::saveAndAssignTemporaryNumbers() {
  mOriginalProductionNumbers.reserve(mElements.size());

  for (size_t i = 0; i < mElements.size(); ++i) {
    const auto& elementId = mElements[i];

    // Save original production number
    const auto originalNumber = mAttributePort->getProductionNumber(elementId);
    mOriginalProductionNumbers[elementId.get()] = originalNumber;

    // Copy production number to assembly number (backup)
    mAttributePort->setAssemblyNumber(elementId, originalNumber);

    // Assign unique temporary production number
    const auto temporaryNumber = generateUniqueNumber(static_cast<uint32_t>(i));
    mAttributePort->setProductionNumber(elementId, temporaryNumber);
  }
}

void ProductionNumberGuard::restoreOriginalNumbers() const {
  for (const auto& elementId : mElements) {
    if (const auto it = mOriginalProductionNumbers.find(elementId.get()); it != mOriginalProductionNumbers.end()) {
      // Restore original production number from stored value
      mAttributePort->setProductionNumber(elementId, it->second);

      // Reset assembly number to zero
      mAttributePort->setAssemblyNumber(elementId, Number{0});
    }
  }
}

Number ProductionNumberGuard::generateUniqueNumber(const uint32_t index) const { return Number{mTemporaryNumberStart + index}; }

}  // namespace borderline::edgeprofile::services
