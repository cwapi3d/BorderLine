#include "SortingStrategyFactory.hh"

#include "SortingStrategies.hh"

namespace borderline::adapter::cadwork {

std::shared_ptr<edgeprofile::ports::ISortingStrategy> SortingStrategyFactory::createStrategy(
    const edgeprofile::ESortingCriteria criteria) const {
    switch (criteria) {
        case edgeprofile::ESortingCriteria::PL_NUMBER:
            return std::make_shared<AssemblyNumberSortingStrategy>();

        case edgeprofile::ESortingCriteria::NESTING_PL_NUMBER:
            return std::make_shared<ProductionNumberSortingStrategy>();

        case edgeprofile::ESortingCriteria::NONE:
        default:
            return std::make_shared<NoSortingStrategy>();
    }
}

}  // namespace borderline::adapter::cadwork
