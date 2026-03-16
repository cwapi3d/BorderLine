#include "EdgeProfileLogic.hh"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ranges>
#include <optional>
#include <string>

#include "IAttributeControlPort.hh"

namespace borderline::edgeprofile
{

namespace {
    void updateEdgePoint(EdgeLine& edge, const bool isStartPoint, const EEdgeType type)
    {
        if (isStartPoint) {
            edge.updateStartPointEdgeType(type);
        } else {
            edge.updateEndPointEdgeType(type);
        }
    }

    struct AffectedPoints {
        bool priorityIsStart;
        bool dependentIsStart;
    };

    AffectedPoints getAffectedPoints(const EEdgePosition priorityPos, const EEdgePosition dependentPos)
    {
        // BOTTOM + RIGHT -> Priority(Start), Dependent(End)
        // BOTTOM + LEFT  -> Priority(End),   Dependent(Start)
        // TOP + LEFT     -> Priority(Start), Dependent(End)
        // TOP + RIGHT    -> Priority(End),   Dependent(Start)

        if (priorityPos == EEdgePosition::BOTTOM) {
            if (dependentPos == EEdgePosition::RIGHT) return {true, false};
            if (dependentPos == EEdgePosition::LEFT)  return {false, true};
        } else if (priorityPos == EEdgePosition::TOP) {
            if (dependentPos == EEdgePosition::LEFT)  return {true, false};
            if (dependentPos == EEdgePosition::RIGHT) return {false, true};
        }
        // Default fallback
        return {false, false};
    }

    struct PriorityTypes {
        EEdgeType priorityType;
        EEdgeType dependentType;
    };

    PriorityTypes getEdgeTypes(const int priority)
    {
        switch (priority) {
            case 1: return {EEdgeType::CONTINOUS, EEdgeType::COUNTERFLOW};
            case 0: return {EEdgeType::COUNTERFLOW, EEdgeType::CONTINOUS};
            case 2: return {EEdgeType::MITER, EEdgeType::MITER};
            default: return {EEdgeType::CONTINOUS, EEdgeType::CONTINOUS};
        }
    }

    std::optional<int> parsePriority(const std::string& val)
    {
        if (val.empty()) return std::nullopt;
        try {
            return std::stoi(val);
        } catch (...) {
            return std::nullopt;
        }
    }
}

EdgeProfileLogic::EdgeProfileLogic(const std::shared_ptr<ports::IAttributeControlPort> &aAttributeControlPort,
                                   const ElementID &aElementId,
                                   const ProfileConfig &aConfig)
    : mAttributeControlPort(aAttributeControlPort),
      mElementID(aElementId),
      mConfig(aConfig)
{
    // Point 520 controls edges 1-4: Edge 1 (BOTTOM) has priority, Edge 4 (RIGHT) end takes opposite
    addRule({
        EEdgePosition::BOTTOM,
        EEdgePosition::RIGHT,
        {aConfig.mBottom.mExistenceAttributeId, aConfig.mRight.mTypeAttributeId},
        EAttributeInterpretation::ControlsCurrentEdgeEnd 
    });

    // Point 508 controls edges 1-2: Edge 1 (BOTTOM) has priority, Edge 2 (LEFT) start takes opposite
    addRule({
        EEdgePosition::BOTTOM,
        EEdgePosition::LEFT,
        {aConfig.mBottom.mExistenceAttributeId, aConfig.mBottom.mTypeAttributeId},
        EAttributeInterpretation::ControlsCurrentEdgeEnd 
    });

    // Point 512 controls edges 3-2: Edge 3 (TOP) has priority, Edge 2 (LEFT) end takes opposite
    addRule({
        EEdgePosition::TOP,
        EEdgePosition::LEFT,
        {aConfig.mTop.mExistenceAttributeId, aConfig.mLeft.mTypeAttributeId},
        EAttributeInterpretation::ControlsCurrentEdgeEnd 
    });

    // Point 516 controls edges 3-4: Edge 3 (TOP) has priority, Edge 4 (RIGHT) start takes opposite
    addRule({
        EEdgePosition::TOP,
        EEdgePosition::RIGHT,
        {aConfig.mTop.mExistenceAttributeId, aConfig.mTop.mTypeAttributeId},
        EAttributeInterpretation::ControlsCurrentEdgeEnd 
    });
}

void EdgeProfileLogic::addRule(const EdgeRule &rule)
{
    mEdgeRules.push_back(rule);
}

void EdgeProfileLogic::determineEdgeLogic(std::map<EEdgePosition, EdgeLine> &edgesMap) const
{
    auto checkAndSet = [&](const EEdgePosition pos, const AttributeIdx attrId) {
        const auto exists = !mAttributeControlPort->getUserAttribute(mElementID, attrId).empty();
        edgesMap.at(pos).setHasEdge(exists);
    };

    checkAndSet(EEdgePosition::BOTTOM, mConfig.mBottom.mExistenceAttributeId);
    checkAndSet(EEdgePosition::LEFT,   mConfig.mLeft.mExistenceAttributeId);
    checkAndSet(EEdgePosition::TOP,    mConfig.mTop.mExistenceAttributeId);
    checkAndSet(EEdgePosition::RIGHT,  mConfig.mRight.mExistenceAttributeId);

    for (const auto &rule : mEdgeRules) {
        applyCornerPriorityLogic(rule, edgesMap);
    }
}

void EdgeProfileLogic::applyCornerPriorityLogic(const EdgeRule &rule, std::map<EEdgePosition, EdgeLine> &edgesMap) const
{
    auto &priorityEdge = edgesMap.at(rule.mPosition);
    auto &dependentEdge = edgesMap.at(rule.mNextPosition);

    if (!priorityEdge.hasEdge()) {
        return;
    }

    const auto priorityValStr = mAttributeControlPort->getUserAttribute(
        mElementID,
        rule.mConfig.mTypeAttributeId);

    const auto [priorityIsStart, dependentIsStart] = getAffectedPoints(rule.mPosition, rule.mNextPosition);

    // If dependent edge doesn't exist, we still update the priority edge to CONTINOUS
    // and the dependent edge (if it exists in map but hasEdge is false) to CONTINOUS as per original logic.
    if (!dependentEdge.hasEdge()) {
        updateEdgePoint(priorityEdge, priorityIsStart, EEdgeType::CONTINOUS);
        updateEdgePoint(dependentEdge, dependentIsStart, EEdgeType::CONTINOUS);
        return;
    }

    const auto priorityOpt = parsePriority(priorityValStr);
    // If invalid or empty -> default to CONTINOUS
    const int priority = priorityOpt.value_or(-1); 

    const auto [priorityType, dependentType] = getEdgeTypes(priority);

    updateEdgePoint(priorityEdge, priorityIsStart, priorityType);
    updateEdgePoint(dependentEdge, dependentIsStart, dependentType);
}

}
