#pragma once

#include <map>
#include <string>

#include "ports/IAttributeControlPort.hh"

namespace borderline::edgeprofile::tests {

class MockAttributeControlPort : public ports::IAttributeControlPort {
 public:
  mutable std::map<std::pair<uint64_t, uint32_t>, std::string> attributes;

  void setAttribute(ElementID elementID, AttributeIdx attributeIdx, const std::string &value) const {
    attributes[{elementID.get(), attributeIdx.get()}] = value;
  }

  std::string getUserAttribute(ElementID elementID, AttributeIdx attributeIdx) const override {
    if (auto it = attributes.find({elementID.get(), attributeIdx.get()}); it != attributes.end()) {
      return it->second;
    }
    return "";
  }

  // Stubs
  Number getProductionNumber(ElementID) const override { return Number(0); }
  Number getAssemblyNumber(ElementID) const override { return Number(0); }
  void setProcessType(ElementID, ports::ProcessType) const override {}
  void setProductionNumber(ElementID, Number) const override {}
  void setAssemblyNumber(ElementID, Number) const override {}
  std::string getAssociatedNestingNumber(ElementID) const override { return ""; }
};

}  // namespace borderline::edgeprofile::tests
