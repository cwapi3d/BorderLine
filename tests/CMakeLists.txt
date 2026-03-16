project(BorderLineTests)

enable_testing()
find_package(doctest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}
        test-main.cc
        EdgeProfileLogicTest.cc
        StandardEdgeOffsetStrategyTest.cc
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        doctest::doctest
        Borderline::EdgeProfile
        Borderline::CadworkAdapter
        CwAPI3D::geometry3d
        CwAPI3D::utility

)

include(doctest)
doctest_discover_tests(${PROJECT_NAME})

#find_package(Catch2 3 REQUIRED)
#
#add_executable(EdgeProfileTests
#        StandardEdgeOffsetStrategyTest.cc
#        EdgeProfileLogicTest.cc
#)
#
#target_link_libraries(EdgeProfileTests
#        PRIVATE
#        Borderline::EdgeProfile
#        CwAPI3D::geometry3d
#        CwAPI3D::utility
#        Catch2::Catch2WithMain
#)
#
#include(Catch)
#catch_discover_tests(EdgeProfileTests)
