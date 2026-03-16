#pragma once

#include <memory>
#include <vector>

#include "IAttributeControlPort.hh"
#include "types/types.hh"

namespace borderline::edgeprofile::services {

class AttributeMappingService final {
public:
    explicit AttributeMappingService(std::shared_ptr<ports::IAttributeControlPort> attributeControlPort);

    void mapAttributesFromPanelToLines(ElementID panelId, const std::vector<ElementID>& lineIds) const;

private:
    std::shared_ptr<ports::IAttributeControlPort> mAttributeControlPort;

    void applyAttributes(ElementID targetId, const std::string& group, const std::string& subgroup) const;
};

}  // namespace borderline::edgeprofile::services
