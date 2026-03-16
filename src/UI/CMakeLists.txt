project(BorderlineUi)

find_package(Qt6
        COMPONENTS
        Core
        Gui
        Widgets
        LinguistTools
        REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(UI_FILES
        cjtcedgesdialog.ui
)

# & "D:\source\cadlib\3rdParty\v_33.0\Qt\x64\bin\lupdate.exe" . -ts ..\..\translations\borderline_de.ts -no-obsolete

set(TS_FILES
        ${CMAKE_SOURCE_DIR}/translations/borderline_de.ts
        ${CMAKE_SOURCE_DIR}/translations/borderline_fr.ts
        ${CMAKE_SOURCE_DIR}/translations/borderline_it.ts
)

# Qt6 command to create .ts files and compile to .qm
qt_add_translations(${PROJECT_NAME}
        TS_FILES ${TS_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
)

# Install .qm files
install(FILES ${QM_FILES}
        DESTINATION translations
)


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_subdirectory(viewmodels)

target_sources(${PROJECT_NAME}
        PRIVATE
        cjtcedgesdialog.cpp
        TranslationManager.cc
        ${UI_FILES}
        PUBLIC
        UISettings.hh
        cjtcedgesdialog.h
        TranslationManager.hh
)

#target_compile_definitions(${PROJECT_NAME} PRIVATE USER_INTERFACE_LIB)

include(GenerateExportHeader)

generate_export_header(${PROJECT_NAME}
        BASE_NAME USER_INTERFACE
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/user_interface/user_interface_export.h

)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/viewmodels>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/include/borderline/user_interface/user_interface_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/borderline/user_interface
)

target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
        Borderline::EdgeProfile
)

if(Qt6LinguistTools_FOUND AND DEFINED TS_FILES)
    # Generate .ts files from source code and update existing translations
    qt_add_lupdate(
        ${PROJECT_NAME}
        TS_FILES ${TS_FILES}
        OPTIONS -no-obsolete
    )
    
    # Compile .ts files to .qm binary files
    qt_add_lrelease(
        ${PROJECT_NAME}
        TS_FILES ${TS_FILES}
        QM_FILES_OUTPUT_VARIABLE QM_FILES
    )
    
    # Install .qm files to translations directory
    install(FILES ${QM_FILES}
        DESTINATION translations
        COMPONENT Runtime
    )
    
    message(STATUS "Translation generation enabled for ${PROJECT_NAME}")
    message(STATUS "  .qm files will be installed to: translations/")
endif()

add_library(Borderline::Ui ALIAS ${PROJECT_NAME})