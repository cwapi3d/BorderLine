cmake_minimum_required(VERSION 3.25)
project(BorderLine
        VERSION 0.0.1
        DESCRIPTION "Borderline - A plugin for cadwork-JTC to create edge profiles for shop drawings"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out/lib/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/out/lib/debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/out/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/out/lib/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/out/lib/release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/out/bin/relwithdebinfo)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/out/lib/relwithdebinfo)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/out/lib/relwithdebinfo)

include(FetchContent)
FetchContent_Declare(
        CwAPI3DEx
        GIT_REPOSITORY https://github.com/Brunner246/cwapi3dcpp-ex.git
        GIT_TAG main
)
# disable tests and examples => for cwapi3dex only
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(CwAPI3DEx)
FetchContent_GetProperties(CwAPI3DEx SOURCE_DIR CwAPI3DEx_source_dir)
message(STATUS "CwAPI3DEx source dir: ${CwAPI3DEx_source_dir}")

message(STATUS "Custom Qt Path from Preset: ${CUSTOM_QT_PATH}")

if(DEFINED CUSTOM_QT_PATH)
    message(STATUS " -> Variable is DEFINED")
else()
    message(STATUS " -> Variable is NOT defined")
endif()

if(DEFINED CUSTOM_QT_PATH)
    message(STATUS "Adding Custom Qt Path: ${CUSTOM_QT_PATH}")
    list(APPEND CMAKE_PREFIX_PATH "${CUSTOM_QT_PATH}")
endif()

# Enable Qt translation tools
find_package(Qt6 COMPONENTS LinguistTools QUIET)
if(Qt6LinguistTools_FOUND)
    message(STATUS "Qt6 LinguistTools found - translation files will be generated")
    
    # Define translation files
    set(TS_FILES
        ${CMAKE_SOURCE_DIR}/translations/borderline_de.ts
        ${CMAKE_SOURCE_DIR}/translations/borderline_fr.ts
        ${CMAKE_SOURCE_DIR}/translations/borderline_it.ts
    )
    
    # Create translations directory if it doesn't exist
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/translations)
    
    message(STATUS "Translation files will be generated for: de, fr, it")
else()
    message(STATUS "Qt6 LinguistTools not found - translation generation disabled")
endif()

add_subdirectory(src/EdgeProfile)
add_subdirectory(src/Adapter/cadwork)
add_subdirectory(src/UI)
add_subdirectory(src/App)

enable_testing()
add_subdirectory(tests)



