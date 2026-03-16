#include "AttributeMappingService.hh"

#include <algorithm>

#include "ports/IAttributeControlPort.hh"

namespace borderline::edgeprofile::services {

AttributeMappingService::AttributeMappingService(std::shared_ptr<ports::IAttributeControlPort> attributeControlPort)
    : mAttributeControlPort(std::move(attributeControlPort)) {
}

void AttributeMappingService::mapAttributesFromPanelToLines(const ElementID panelId,
                                                            const std::vector<ElementID>& lineIds) const {
    const auto group = mAttributeControlPort->getGroup(panelId);
    const auto subgroup = mAttributeControlPort->getSubgroup(panelId);

    std::ranges::for_each(
        lineIds, [this, &group, &subgroup](const ElementID lineId) { applyAttributes(lineId, group, subgroup); });
}

void AttributeMappingService::applyAttributes(const ElementID targetId, const std::string& group,
                                              const std::string& subgroup) const {
    mAttributeControlPort->setGroup(targetId, group);
    mAttributeControlPort->setSubgroup(targetId, subgroup);
}

}  // namespace borderline::edgeprofile::services
