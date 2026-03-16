#include "ElementGroupProcessingService.hh"

#include <algorithm>
#include <map>
#include <ranges>
#include <stdexcept>

namespace borderline::edgeprofile::services {

ElementGroupProcessingService::ElementGroupProcessingService(std::shared_ptr<ports::IAttributeControlPort> attributePort,
                                                             std::shared_ptr<ports::ISortingStrategy> sortingStrategy,
                                                             const ESortingCriteria sortingCriteria)
    : mAttributePort(std::move(attributePort))
    , mSortingStrategy(std::move(sortingStrategy))
    , mSortingCriteria(sortingCriteria) {}

std::vector<std::vector<ElementID>> ElementGroupProcessingService::sortAndChunk(
    const std::unordered_map<std::string, std::vector<ElementID>>& data, const int pageLimit) const {
  std::vector<ElementID> processedElements;

  switch (mSortingCriteria) {
    case ESortingCriteria::NESTING_PL_NUMBER:
      processedElements = processByNestingAndPlNumber(data);
      break;

    case ESortingCriteria::PL_NUMBER:
      processedElements = processByPlNumberOnly(data);
      break;

    case ESortingCriteria::NONE:
      processedElements = flattenWithoutSorting(data);
      break;
  }

  return chunkElements(processedElements, pageLimit);
}

std::vector<ElementID> ElementGroupProcessingService::processByNestingAndPlNumber(
    const std::unordered_map<std::string, std::vector<ElementID>>& data) const {
  // Group by nesting parent (sort groups numerically)
  std::map<int, std::vector<ElementID>> sortedGroups;
  for (const auto& [key, value] : data) {
    try {
      sortedGroups[std::stoi(key)] = value;
    } catch (const std::invalid_argument& e) {
      std::ignore = e;
      sortedGroups[0] = value;
    }
  }

  std::ranges::for_each(sortedGroups | std::views::values, [this](auto& elements) { mSortingStrategy->sort(elements, *mAttributePort); });

  std::vector<ElementID> result;
  for (const auto& elements : sortedGroups | std::views::values) {
    result.insert(result.end(), elements.begin(), elements.end());
  }

  return result;
}

std::vector<ElementID> ElementGroupProcessingService::processByPlNumberOnly(
    const std::unordered_map<std::string, std::vector<ElementID>>& data) const {
  // Flatten all elements ignoring nesting parent grouping
  std::vector<ElementID> allElements;
  for (const auto& elements : data | std::views::values) {
    allElements.insert(allElements.end(), elements.begin(), elements.end());
  }

  // Sort by production number only
  mSortingStrategy->sort(allElements, *mAttributePort);

  return allElements;
}

std::vector<ElementID> ElementGroupProcessingService::flattenWithoutSorting(
    const std::unordered_map<std::string, std::vector<ElementID>>& data) {
  std::vector<ElementID> result;
  for (const auto& elements : data | std::views::values) {
    result.insert(result.end(), elements.begin(), elements.end());
  }
  return result;
}

std::vector<std::vector<ElementID>> ElementGroupProcessingService::chunkElements(const std::vector<ElementID>& elements,
                                                                                 const int pageLimit) {
  if (elements.empty()) {
    return {};
  }

  if (pageLimit <= 0) {
    return {elements};
  }

  std::vector<std::vector<ElementID>> chunks;
  for (auto i = 0; i < elements.size(); i += pageLimit) {
    const auto end = std::min(i + static_cast<size_t>(pageLimit), elements.size());
    chunks.emplace_back(elements.begin() + i, elements.begin() + end);
  }

  return chunks;
}

}  // namespace borderline::edgeprofile::services
