#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <borderline/edge_profile/edge_profile_export.h>

#include "ports/IAttributeControlPort.hh"
#include "types/types.hh"

namespace borderline::edgeprofile::services {

/**
 * @brief RAII guard for temporarily modifying production numbers during export.
 * 
 * This guard saves the original production numbers to assembly numbers,
 * assigns unique temporary production numbers starting from 40000,
 * and automatically restores the original state when destroyed.
 * 
 * Usage:
 * {
 *   ProductionNumberGuard guard(attributePort, elements);
 *   // Export operations use temporary numbers
 * } // Automatic restoration on scope exit
 */
class EDGE_PROFILE_EXPORT ProductionNumberGuard final {
public:
    explicit ProductionNumberGuard(
        std::shared_ptr<ports::IAttributeControlPort> attributePort,
        const std::vector<ElementID>& elements,
        uint32_t temporaryNumberStart);

    ~ProductionNumberGuard();

    // Delete copy and move to ensure single ownership
    ProductionNumberGuard(const ProductionNumberGuard&) = delete;
    ProductionNumberGuard(ProductionNumberGuard&&) = delete;
    ProductionNumberGuard& operator=(const ProductionNumberGuard&) = delete;
    ProductionNumberGuard& operator=(ProductionNumberGuard&&) = delete;

private:
    void saveAndAssignTemporaryNumbers();
    void restoreOriginalNumbers() const;
    
    [[nodiscard]] Number generateUniqueNumber(uint32_t index) const;

    std::shared_ptr<ports::IAttributeControlPort> mAttributePort;
    std::vector<ElementID> mElements;
    std::unordered_map<uint64_t, Number> mOriginalProductionNumbers;
    
    uint32_t mTemporaryNumberStart;
};

}  // namespace borderline::edgeprofile::services
