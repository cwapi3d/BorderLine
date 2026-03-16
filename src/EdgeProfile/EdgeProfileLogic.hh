//
// Created by MichaelBrunner on 08/12/2025.
//

#pragma once

#include <map>
#include <memory>
#include <vector>
#include <borderline/edge_profile/edge_profile_export.h>

#include "EdgeLine.hh"
#include "types/types.hh"

/*
*EdgeProfileLogic::ProfileConfig config{
.mBottom = { .mExistenceAttributeId = AttributeIdx{505}, .mTypeAttributeId = AttributeIdx{508} },
.mLeft   = { .mExistenceAttributeId = AttributeIdx{509}, .mTypeAttributeId = AttributeIdx{512} },
.mTop    = { .mExistenceAttributeId = AttributeIdx{513}, .mTypeAttributeId = AttributeIdx{516} },
.mRight  = { .mExistenceAttributeId = AttributeIdx{517}, .mTypeAttributeId = AttributeIdx{520} }
};

auto logic = std::make_unique<EdgeProfileLogic>(controlPort, elementId, config);
*/

namespace borderline::edgeprofile
{
namespace ports
{
class IAttributeControlPort;
}

enum class EAttributeInterpretation
{
    ControlsCurrentEdgeEnd,
    ControlsNextEdgeStart
};

struct EdgeConfig
{
    AttributeIdx mExistenceAttributeId;
    AttributeIdx mTypeAttributeId;
};

struct EdgeRule
{
    EEdgePosition mPosition;
    EEdgePosition mNextPosition;
    EdgeConfig mConfig; // Replaces the individual ID fields
    EAttributeInterpretation mAttributeInterpretation;
};

struct EdgeAttributeIds
{
    AttributeIdx mExistenceAttributeId;
    AttributeIdx mTypeAttributeId;
};

struct ProfileConfig
{
    EdgeAttributeIds mBottom;
    EdgeAttributeIds mLeft;
    EdgeAttributeIds mTop;
    EdgeAttributeIds mRight;
    EEdgeSymbolSize mSymbolSize = EEdgeSymbolSize::MEDIUM;
    bool mCreateEdges = true;
    bool mCreateSymbol = true;
};

class EDGE_PROFILE_EXPORT EdgeProfileLogic
{
public:
    EdgeProfileLogic(const std::shared_ptr<ports::IAttributeControlPort> &aAttributeControlPort,
                     const ElementID &aElementId,
                     const ProfileConfig &aConfig);

    void determineEdgeLogic(std::map<EEdgePosition, EdgeLine> &edgesMap) const;

private:
    void addRule(const EdgeRule &rule);

    void applyCornerPriorityLogic(const EdgeRule &rule, std::map<EEdgePosition, EdgeLine> &edgesMap) const;

    std::shared_ptr<ports::IAttributeControlPort> mAttributeControlPort;
    ElementID mElementID;
    ProfileConfig mConfig;
    std::vector<EdgeRule> mEdgeRules;
};
}
