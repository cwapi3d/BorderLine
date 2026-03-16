#pragma once

#include <memory>
#include "ISortingStrategy.hh"
#include "utility/utility_types.hh"

namespace borderline::edgeprofile::ports {

/**
 * @brief Factory interface for creating sorting strategies.
 * 
 * This port allows the core to request sorting strategies without
 * knowing about concrete implementations. The adapter layer provides
 * the concrete factory implementation.
 */
class ISortingStrategyFactory {
 public:
  virtual ~ISortingStrategyFactory() = default;

  /**
   * @brief Creates a sorting strategy based on the specified criteria.
   * 
   * @param criteria The sorting criteria determining which strategy to create
   * @return A shared pointer to the created strategy
   */
  [[nodiscard]] virtual std::shared_ptr<ISortingStrategy> createStrategy(
      ESortingCriteria criteria) const = 0;
};

}  // namespace borderline::edgeprofile::ports
