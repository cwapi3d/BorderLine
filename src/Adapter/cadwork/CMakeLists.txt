project(BorderlineCadworkAdapter)

find_package(CwAPI3D CONFIG REQUIRED)
find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(${PROJECT_NAME} SHARED)


include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
        BASE_NAME CADWORK_ADAPTER
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/adapter/cadwork/cadwork_adapter_export.h

)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/adapter/cadwork/cadwork_adapter_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/borderline/adapter/cadwork
)

add_library(Borderline::CadworkAdapter ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PUBLIC
        ElementControl.hh
        AttributeControl.hh
        GeometryControl.hh
        ShopDrawingControl.hh
        UtilityControl.hh
        VisualizationControl.hh
        config/ConfigurationAdapter.hh
        strategies/SortingStrategies.hh
        strategies/SortingStrategyFactory.hh
        strategies/StandardTriangleStrategy.hh
        strategies/StandardEdgeOffsetStrategy.hh
        PRIVATE
        ElementControl.cc
        AttributeControl.cc
        GeometryControl.cc
        ShopDrawingControl.cc
        UtilityControl.cc
        VisualizationControl.cc
        config/ConfigurationAdapter.cc
        strategies/SortingStrategyFactory.cc
        strategies/StandardTriangleStrategy.cc
        strategies/StandardEdgeOffsetStrategy.cc

)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Borderline::EdgeProfile
        CwAPI3D::CwAPI3D
        Qt6::Core
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        CWAPI3D_PLUGIN_NAME=L"Borderline Cadwork Adapter"
        CWAPI3D_AUTHOR_NAME=L"Michael Brunner"
        CWAPI3D_AUTHOR_EMAIL=L"brunner@cadwork.swiss"
)

