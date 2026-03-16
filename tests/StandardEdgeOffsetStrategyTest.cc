#include <doctest/doctest.h>

#include "EdgeLine.hh"
#include "cwapi3d/geometry/CoordinateFrame3D.h"
#include "strategies/StandardEdgeOffsetStrategy.hh"

using namespace borderline::adapter::cadwork;
using namespace borderline::edgeprofile;

TEST_CASE("StandardEdgeOffsetStrategy ApplyOffsets") {
    StandardEdgeOffsetStrategy strategy;
    EdgeOffsetConfig config;
    
    // Setup config with known values
    config.setGeneralOffset(1.0);
    config.setCounterflowDistance(2.0);
    config.setContinousDistance(0.5);
    config.setMiterDistance(1.5);

    // Setup a standard coordinate frame (Identity)
    auto coordFrame = CwAPI3D::Geometry::CoordinateFrame3D(
        CwAPI3D::Geometry::Point3D(0, 0, 0),
        CwAPI3D::Geometry::Vec3D(1, 0, 0),
        CwAPI3D::Geometry::Vec3D(0, 1, 0),
        CwAPI3D::Geometry::Vec3D(0, 0, 1)
    );

    SUBCASE("Bottom Edge Continuous") {
        std::map<EEdgePosition, EdgeLine> edges;
        EdgeLine edge;
        edge.setHasEdge(true);
        
        auto p520 = CwAPI3D::Geometry::Point3D(0, -5, 0);
        auto p508 = CwAPI3D::Geometry::Point3D(0, 5, 0);

        edge.setStartPoint({p520, EEdgeVerticeID::PT520, EEdgeType::CONTINOUS});
        edge.setEndPoint({p508, EEdgeVerticeID::PT508, EEdgeType::CONTINOUS});
        
        edges[EEdgePosition::BOTTOM] = edge;

        strategy.applyOffsets(edges, coordFrame, config);

        const auto& modifiedEdge = edges[EEdgePosition::BOTTOM];
        const auto& start = modifiedEdge.getStartPoint().getPosition();
        const auto& end = modifiedEdge.getEndPoint().getPosition();

        REQUIRE(start.x() == doctest::Approx(1.0).epsilon(0.001));
        REQUIRE(start.y() == doctest::Approx(-4.5).epsilon(0.001));
        REQUIRE(start.z() == doctest::Approx(0.0).epsilon(0.001));

        REQUIRE(end.x() == doctest::Approx(1.0).epsilon(0.001));
        REQUIRE(end.y() == doctest::Approx(4.5).epsilon(0.001));
        REQUIRE(end.z() == doctest::Approx(0.0).epsilon(0.001));
    }

    SUBCASE("Bottom Edge Counterflow") {
        std::map<EEdgePosition, EdgeLine> edges;
        EdgeLine edge;
        edge.setHasEdge(true);
        
        auto p520 = CwAPI3D::Geometry::Point3D(0, -5, 0);
        auto p508 = CwAPI3D::Geometry::Point3D(0, 5, 0);

        edge.setStartPoint({p520, EEdgeVerticeID::PT520, EEdgeType::COUNTERFLOW});
        edge.setEndPoint({p508, EEdgeVerticeID::PT508, EEdgeType::COUNTERFLOW});
        
        edges[EEdgePosition::BOTTOM] = edge;

        strategy.applyOffsets(edges, coordFrame, config);

        const auto& modifiedEdge = edges[EEdgePosition::BOTTOM];
        const auto& start = modifiedEdge.getStartPoint().getPosition();
        const auto& end = modifiedEdge.getEndPoint().getPosition();

        REQUIRE(start.x() == doctest::Approx(1.0).epsilon(0.001));
        REQUIRE(start.y() == doctest::Approx(-2.0).epsilon(0.001));
        REQUIRE(start.z() == doctest::Approx(0.0).epsilon(0.001));

        REQUIRE(end.x() == doctest::Approx(1.0).epsilon(0.001));
        REQUIRE(end.y() == doctest::Approx(6.0).epsilon(0.001));
        REQUIRE(end.z() == doctest::Approx(0.0).epsilon(0.001));
    }
}
