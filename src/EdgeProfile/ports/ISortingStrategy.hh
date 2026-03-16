#pragma once

#include <vector>
#include "types/types.hh"
#include "IAttributeControlPort.hh"

namespace borderline::edgeprofile::ports {

class ISortingStrategy {
public:
    virtual ~ISortingStrategy() = default;
    
    // Sorts a vector of elements.
    virtual void sort(std::vector<ElementID>& elements, 
                      const IAttributeControlPort& attrPort) const = 0;
};

}
