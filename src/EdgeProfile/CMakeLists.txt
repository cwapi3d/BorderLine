project(BorderlineEdgeProfile)

add_library(${PROJECT_NAME} SHARED)


include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
        BASE_NAME EDGE_PROFILE
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/edge_profile/edge_profile_export.h

)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ports>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#        ${CMAKE_CURRENT_SOURCE_DIR}
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/edge_profile/edge_profile_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/borderline/edge_profile
)

target_sources(${PROJECT_NAME}
        PRIVATE
        EdgeProfileLogic.cc
        CabinetAggregate.cc
        CabinetElement.cc
        CabinetElementAttributes.cc
        CabinetElementGeometry.cc
        CabinetBuilder.cc
        EdgeGeometryUtils.cc
        utility/VectorAlignmentUtility.cc

        PUBLIC
        EdgeProfileLogic.hh
        EdgeLine.hh
        EdgeProfilePositionData.hh
        utility/utility_types.hh
        utility/VectorAlignmentUtility.hh
        CabinetAggregate.hh
        CabinetElement.hh
        CabinetElementAttributes.hh
        CabinetElementGeometry.hh
        CabinetBuilder.hh
        CabinetNode.hh
        EdgeGeometryUtils.hh
)

add_subdirectory(ports)
add_subdirectory(types)
add_subdirectory(services)

add_library(Borderline::EdgeProfile ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
        PUBLIC # targets that link this get these too :)
        CwAPI3D::geometry3d
        CwAPI3D::composite
        CwAPI3D::utility
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
)
