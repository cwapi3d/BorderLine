project(BorderlineApp)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)
find_package(CwAPI3D CONFIG REQUIRED)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
        PRIVATE
        bootstrap.cc
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        CwAPI3D::CwAPI3D
        CwAPI3D::geometry3d
        CwAPI3D::utility
        Borderline::CadworkAdapter
        Borderline::EdgeProfile
        Borderline::Ui
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)


add_library(Borderline::App ALIAS ${PROJECT_NAME})


target_compile_definitions(${PROJECT_NAME} PRIVATE
        CWAPI3D_PLUGIN_NAME=L"Cadwork Test Plugin"
        CWAPI3D_AUTHOR_NAME=L"Your Name"
        CWAPI3D_AUTHOR_EMAIL=L"your_name@something.swiss"
)
