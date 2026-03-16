//
// Created by MichaelBrunner on 09/12/2025.
//

#pragma once

#include "EdgeProfileLogic.hh"
#include "services/EdgeImageService.hh"

namespace borderline::edgeprofile::ports {

class ElementGroupings {
public:
    explicit ElementGroupings(std::unordered_map<std::string, std::vector<ElementID>> elementsByNestingAssociation,
                              std::unordered_map<uint64_t, std::vector<ElementID>> edgeLinesByPanelId)
        : mElementsByNestingAssociation(std::move(elementsByNestingAssociation)),
          mEdgeLinesByPanelId(std::move(edgeLinesByPanelId)) {}

    [[nodiscard]] const std::unordered_map<std::string, std::vector<ElementID>>& getElementsByNestingAssociation()
        const& {
        return mElementsByNestingAssociation;
    }

    [[nodiscard]] const std::unordered_map<uint64_t, std::vector<ElementID>>& getEdgeLinesByPanelId() const& {
        return mEdgeLinesByPanelId;
    }

private:
    std::unordered_map<std::string, std::vector<ElementID>> mElementsByNestingAssociation;
    std::unordered_map<uint64_t, std::vector<ElementID>> mEdgeLinesByPanelId;
};

class IEdgeImageGenerator {
public:
    virtual ~IEdgeImageGenerator() = default;

    virtual ElementGroupings createEdgeImageLinesAndGroupCabinElementsByNestingParent(
        const ProfileConfig& config, const std::vector<ElementID>& aElementSelection) = 0;
};
}  // namespace borderline::edgeprofile::ports
