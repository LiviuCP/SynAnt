cmake_minimum_required(VERSION 3.14)

project(Tests VERSION 2.1 LANGUAGES CXX)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

include_directories(
    ../SystemFunctionality/CoreFunctionality
    ../SystemFunctionality/DataAccess
    ../SystemFunctionality/Utilities
)

add_executable(CoreFunctionalityTests
    tst_corefunctionalitytests.cpp
)

add_executable(DataAccessTests
    tst_dataaccesstests.cpp
)

add_executable(DataEntryTests
    tst_dataentrytests.cpp
)

add_executable(UtilitiesTests
    tst_utilitiestests.cpp
)

add_test(NAME CoreFunctionalityTests COMMAND CoreFunctionalityTests)
add_test(NAME DataAccessTests COMMAND DataAccessTests)
add_test(NAME DataEntryTests COMMAND DataEntryTests)
add_test(NAME UtilitiesTests COMMAND UtilitiesTests)

target_link_libraries(CoreFunctionalityTests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    ${SYS_FUNC_LIB_NAME}
)

target_link_libraries(DataAccessTests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    ${SYS_FUNC_LIB_NAME}
)

target_link_libraries(DataEntryTests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    ${SYS_FUNC_LIB_NAME}
)

target_link_libraries(UtilitiesTests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    ${SYS_FUNC_LIB_NAME}
)
