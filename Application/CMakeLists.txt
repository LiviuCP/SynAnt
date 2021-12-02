cmake_minimum_required(VERSION 3.14)

project(${APP_LIB_NAME} VERSION 2.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

set(APP_LIB_SOURCES
    Presentation/gamepresenter.cpp
    Presentation/dataentrypresenter.cpp
)

include_directories(
    Presentation
    Utilities
    ../SystemFunctionality/Management
    ../SystemFunctionality/ManagementProxies
    ../SystemFunctionality/Utilities
)

add_library(${APP_LIB_NAME} STATIC
    ${APP_LIB_SOURCES}
)

target_link_libraries(${APP_LIB_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick # required for <QGuiApplication> in game presenter
    ${SYS_FUNC_LIB_NAME}
)

target_compile_definitions(${APP_LIB_NAME} PRIVATE
    APPLICATION_LIBRARY
)
