#include <doctest/doctest.h>
#include "EdgeProfileLogic.hh"
#include "Mocks.hh"

using namespace borderline::edgeprofile;
using namespace borderline::edgeprofile::tests;

TEST_CASE("EdgeProfileLogic DetermineEdgeLogic") {
    auto mockControlPort = std::make_shared<MockAttributeControlPort>();
    ElementID elementId{123};
    ProfileConfig config;
    std::map<EEdgePosition, EdgeLine> edges;

    // Setup Config
    config.mBottom = { AttributeIdx{505}, AttributeIdx{508} };
    config.mLeft   = { AttributeIdx{509}, AttributeIdx{512} };
    config.mTop    = { AttributeIdx{513}, AttributeIdx{516} };
    config.mRight  = { AttributeIdx{517}, AttributeIdx{520} };

    // Initialize Edges
    edges[EEdgePosition::BOTTOM] = EdgeLine();
    edges[EEdgePosition::LEFT] = EdgeLine();
    edges[EEdgePosition::TOP] = EdgeLine();
    edges[EEdgePosition::RIGHT] = EdgeLine();
    
    // Initialize points with default types
    edges[EEdgePosition::BOTTOM].setStartPoint({{}, EEdgeVerticeID::PT520, EEdgeType::CONTINOUS});
    edges[EEdgePosition::BOTTOM].setEndPoint({{}, EEdgeVerticeID::PT508, EEdgeType::CONTINOUS});
    
    edges[EEdgePosition::RIGHT].setStartPoint({{}, EEdgeVerticeID::PT516, EEdgeType::CONTINOUS});
    edges[EEdgePosition::RIGHT].setEndPoint({{}, EEdgeVerticeID::PT520, EEdgeType::CONTINOUS});

    SUBCASE("Bottom Right Priority 1") {
        // Setup: Bottom and Right edges exist
        mockControlPort->setAttribute(elementId, config.mBottom.mExistenceAttributeId, "exists");
        mockControlPort->setAttribute(elementId, config.mRight.mExistenceAttributeId, "exists");
        mockControlPort->setAttribute(elementId, config.mLeft.mExistenceAttributeId, "");
        mockControlPort->setAttribute(elementId, config.mTop.mExistenceAttributeId, "");

        // Setup: Priority 1 for Bottom-Right corner
        mockControlPort->setAttribute(elementId, config.mRight.mTypeAttributeId, "1");

        EdgeProfileLogic logic(mockControlPort, elementId, config);
        logic.determineEdgeLogic(edges);

        // Check Existence
        REQUIRE(edges[EEdgePosition::BOTTOM].hasEdge());
        REQUIRE(edges[EEdgePosition::RIGHT].hasEdge());
        REQUIRE_FALSE(edges[EEdgePosition::LEFT].hasEdge());
        REQUIRE_FALSE(edges[EEdgePosition::TOP].hasEdge());

        // Check Priority Logic for Bottom-Right (PT520)
        // Priority 1 -> Priority Edge (Bottom) gets CONTINOUS, Dependent Edge (Right) gets COUNTERFLOW
        
        REQUIRE(edges[EEdgePosition::BOTTOM].getStartPoint().getEdgeType() == EEdgeType::CONTINOUS);
        REQUIRE(edges[EEdgePosition::RIGHT].getEndPoint().getEdgeType() == EEdgeType::COUNTERFLOW);
    }

    SUBCASE("Bottom Right Priority 0") {
        // Setup: Bottom and Right edges exist
        mockControlPort->setAttribute(elementId, config.mBottom.mExistenceAttributeId, "exists");
        mockControlPort->setAttribute(elementId, config.mRight.mExistenceAttributeId, "exists");
        mockControlPort->setAttribute(elementId, config.mLeft.mExistenceAttributeId, "");
        mockControlPort->setAttribute(elementId, config.mTop.mExistenceAttributeId, "");

        // Setup: Priority 0 for Bottom-Right corner
        mockControlPort->setAttribute(elementId, config.mRight.mTypeAttributeId, "0");

        EdgeProfileLogic logic(mockControlPort, elementId, config);
        logic.determineEdgeLogic(edges);

        // Check Priority Logic for Bottom-Right (PT520)
        // Priority 0 -> Priority Edge (Bottom) gets COUNTERFLOW, Dependent Edge (Right) gets CONTINOUS
        
        REQUIRE(edges[EEdgePosition::BOTTOM].getStartPoint().getEdgeType() == EEdgeType::COUNTERFLOW);
        REQUIRE(edges[EEdgePosition::RIGHT].getEndPoint().getEdgeType() == EEdgeType::CONTINOUS);
    }
}
