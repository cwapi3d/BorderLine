#pragma once

#include <borderline/adapter/cadwork/cadwork_adapter_export.h>

#include <memory>
#include <ports/ISortingStrategy.hh>
#include <ports/ISortingStrategyFactory.hh>
#include <utility/utility_types.hh>

namespace borderline::adapter::cadwork {

class CADWORK_ADAPTER_EXPORT SortingStrategyFactory final : public edgeprofile::ports::ISortingStrategyFactory {
public:
    [[nodiscard]] std::shared_ptr<edgeprofile::ports::ISortingStrategy> createStrategy(
        edgeprofile::ESortingCriteria criteria) const override;
};

}  // namespace borderline::adapter::cadwork
