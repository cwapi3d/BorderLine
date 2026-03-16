#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <borderline/edge_profile/edge_profile_export.h>

#include "types/types.hh"
#include "IAttributeControlPort.hh"
#include "ISortingStrategy.hh"
#include "../utility/utility_types.hh"

namespace borderline::edgeprofile::services {

class EDGE_PROFILE_EXPORT ElementGroupProcessingService {
public:
    explicit ElementGroupProcessingService(
        std::shared_ptr<ports::IAttributeControlPort> attributePort,
        std::shared_ptr<ports::ISortingStrategy> sortingStrategy,
        ESortingCriteria sortingCriteria);

    [[nodiscard]] std::vector<std::vector<ElementID>> sortAndChunk(
        const std::unordered_map<std::string, std::vector<ElementID>>& data,
        int pageLimit) const;

private:
    [[nodiscard]] std::vector<ElementID> processByNestingAndPlNumber(
        const std::unordered_map<std::string, std::vector<ElementID>>& data) const;
    
    [[nodiscard]] std::vector<ElementID> processByPlNumberOnly(
        const std::unordered_map<std::string, std::vector<ElementID>>& data) const;
    
    [[nodiscard]] static std::vector<ElementID> flattenWithoutSorting(
        const std::unordered_map<std::string, std::vector<ElementID>>& data);
    
    [[nodiscard]] static std::vector<std::vector<ElementID>> chunkElements(
        const std::vector<ElementID>& elements, int pageLimit);

private:
    std::shared_ptr<ports::IAttributeControlPort> mAttributePort;
    std::shared_ptr<ports::ISortingStrategy> mSortingStrategy;
    ESortingCriteria mSortingCriteria;
};

}
