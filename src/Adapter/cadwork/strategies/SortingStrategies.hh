#pragma once

#include <algorithm>
#include <ports/IAttributeControlPort.hh>
#include <ports/ISortingStrategy.hh>
#include <ranges>
#include <types/types.hh>

namespace borderline::adapter::cadwork {

/**
 * @brief No-operation sorting strategy (elements remain in original order).
 */
class NoSortingStrategy final : public edgeprofile::ports::ISortingStrategy {
public:
    void sort(std::vector<edgeprofile::ElementID>& elements,
              const edgeprofile::ports::IAttributeControlPort& attrPort) const override {
        (void)elements;
        (void)attrPort;
    }
};

/**
 * @brief Sorts elements by Assembly Number in ascending order.
 */
class AssemblyNumberSortingStrategy final : public edgeprofile::ports::ISortingStrategy {
public:
    void sort(std::vector<edgeprofile::ElementID>& elements,
              const edgeprofile::ports::IAttributeControlPort& attrPort) const override {
        std::ranges::sort(elements, [&](const auto& idA, const auto& idB) {
            const auto numA = attrPort.getAssemblyNumber(idA).get();
            const auto numB = attrPort.getAssemblyNumber(idB).get();
            return numA < numB;
        });
    }
};

/**
 * @brief Sorts elements by Production Number in ascending order.
 */
class ProductionNumberSortingStrategy final : public edgeprofile::ports::ISortingStrategy {
public:
    void sort(std::vector<edgeprofile::ElementID>& elements,
              const edgeprofile::ports::IAttributeControlPort& attrPort) const override {
        std::ranges::sort(elements, [&](const auto& idA, const auto& idB) {
            const auto numA = attrPort.getProductionNumber(idA).get();
            const auto numB = attrPort.getProductionNumber(idB).get();
            return numA < numB;
        });
    }
};

}  // namespace borderline::adapter::cadwork
