cmake_minimum_required(VERSION 3.14)

project(SystemFunctionality VERSION 2.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Sql REQUIRED)

set(PROJECT_SOURCES
    CoreFunctionality/wordmixer.cpp
    CoreFunctionality/wordpairowner.cpp
    CoreFunctionality/inputbuilder.cpp
    DataAccess/datasource.cpp
    DataAccess/datasourceloader.cpp
    DataAccess/dataentryvalidator.cpp
    DataAccess/dataentrycache.cpp
    DataAccess/dataentrystatistics.cpp
    DataAccess/datasourceaccesshelper.cpp
    ManagementInterfaces/datainterface.cpp
    ManagementInterfaces/dataentryinterface.cpp
    ManagementInterfaces/gameinterface.cpp
    ManagementInterfaces/gameinitinterface.cpp
    ManagementInterfaces/gamefunctionalityinterface.cpp
    Management/gamemanager.cpp
    Management/gamefacade.cpp
    Management/dataentryfacade.cpp
    ManagementProxies/gameinitproxy.cpp
    ManagementProxies/gamefunctionalityproxy.cpp
    ManagementProxies/gameproxy.cpp
    ManagementProxies/dataproxy.cpp
    ManagementProxies/dataentryproxy.cpp
    Utilities/statisticsitem.cpp
    Utilities/chronometer.cpp
    Utilities/exceptions.cpp
    systemfunctionality.cpp
)

include_directories(
    CoreFunctionality
    DataAccess
    ManagementInterfaces
    Management
    ManagementProxies
    Utilities
)

set(SYS_FUNC_LIB_TYPE SHARED)

# Some errors were encountered when building SystemFunctionality as shared library on Windows so it has been decided to keep it static on this OS for the moment
if(WIN32)
    set(SYS_FUNC_LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${SYS_FUNC_LIB_TYPE}
    ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Sql
)

target_compile_definitions(SystemFunctionality PRIVATE
    SYSTEMFUNCTIONALITY_LIBRARY
)
