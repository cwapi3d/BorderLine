//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once
#include <string>

#include "types/types.hh"

namespace borderline::edgeprofile::ports {
enum class ProcessType {
    Panel_1,
};

class IAttributeControlPort {
public:
    virtual ~IAttributeControlPort() = default;

    [[nodiscard]] virtual auto getUserAttribute(ElementID elementID, AttributeIdx attributeIdx) const -> std::string
        = 0;

    [[nodiscard]] virtual Number getProductionNumber(ElementID elementID) const = 0;
    virtual void setProductionNumber(ElementID elementID, Number productionNumber) const = 0;

    [[nodiscard]] virtual Number getAssemblyNumber(ElementID elementID) const = 0;
    virtual void setAssemblyNumber(ElementID elementID, Number assemblyNumber) const = 0;

    virtual void setProcessType(ElementID elementID, ProcessType processType) const = 0;

    [[nodiscard]] virtual std::string getAssociatedNestingNumber(ElementID elementID) const = 0;

    [[nodiscard]] virtual std::string getSubgroup(ElementID elementID) const = 0;
    virtual void setSubgroup(ElementID elementID, std::string subgroup) const = 0;

    [[nodiscard]] virtual std::string getGroup(ElementID elementID) const = 0;
    virtual void setGroup(ElementID elementID, std::string group) const = 0;
};
}  // namespace borderline::edgeprofile::ports
