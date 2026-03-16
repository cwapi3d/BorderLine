#pragma once

#include "types/types.hh"

namespace borderline::edgeprofile::ports {

class IUtilityControlPort {
public:
    virtual ~IUtilityControlPort() = default;

    [[nodiscard]] virtual uint32_t getUserColor(Number colorNumber) const = 0;
};

}  // namespace borderline::edgeprofile::ports
