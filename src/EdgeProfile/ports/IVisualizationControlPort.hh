//
// Created by MichaelBrunner on 15/12/2025.
//

#pragma once
#include <vector>

#include "types/types.hh"

namespace borderline::edgeprofile::ports {
class IVisualizationControlPort {
public:
    virtual ~IVisualizationControlPort() = default;

    virtual void setColor(const std::vector<ElementID> &elementIDs, Number colorNumber) = 0;

    struct RGB {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    [[nodiscard]] virtual RGB getRgbColor(Number colorNumber) const = 0;
};
}  // namespace borderline::edgeprofile::ports