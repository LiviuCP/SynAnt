cmake_minimum_required(VERSION 3.14)

set(APP_NAME SynAnt)
set(APP_LIB_NAME Application)
set(SYS_FUNC_LIB_NAME SystemFunctionality)

project(${APP_NAME} VERSION 2.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
    main.cpp
    register.cpp
    ${APP_LIB_NAME}/QML/qml.qrc
)

add_subdirectory(${APP_LIB_NAME})
add_subdirectory(${SYS_FUNC_LIB_NAME})

if (CMAKE_BUILD_TYPE MATCHES Debug)
   add_subdirectory(Tests)
endif()

include_directories(
    Application/Presentation
    SystemFunctionality/ManagementProxies
    SystemFunctionality/Utilities
)

set(ADDITIONAL_BUILD_OPTIONS "")

if (APPLE)
    set(ADDITIONAL_BUILD_OPTIONS "MACOSX_BUNDLE")
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP_NAME} ${ADDITIONAL_BUILD_OPTIONS}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(${APP_NAME} ${ADDITIONAL_BUILD_OPTIONS}
        ${PROJECT_SOURCES}
    )
endif()

target_compile_definitions(${APP_NAME} PRIVATE
    $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>
)

target_link_libraries(${APP_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    ${APP_LIB_NAME}
    ${SYS_FUNC_LIB_NAME}
)

set_target_properties(${APP_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${APP_NAME})
    qt_finalize_executable(${APP_NAME})
endif()

# this is required for Linux systems when using linuxdeployqt for deploying SynAnt:
# - in order to be able to run the app after deployment it should be ensured that the system functionality library (and any other shared library in general) had previously been copied into the "lib" subdir
# - the deploy tool will subsequently append all required Qt libraries to this subfolder
if (UNIX AND NOT APPLE)
    add_custom_target(make-shared-lib-directory ALL
        COMMAND ${CMAKE_COMMAND} -E make_directory lib
    )

    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${SYS_FUNC_LIB_NAME}/lib${SYS_FUNC_LIB_NAME}.so" lib
    )
endif()
