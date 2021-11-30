cmake_minimum_required(VERSION 3.14)

set(APP_NAME SynAnt)
set(SYS_FUNC_LIB_NAME SystemFunctionality)

project(${APP_NAME}App VERSION 2.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Application)
add_subdirectory(${SYS_FUNC_LIB_NAME})

if (CMAKE_BUILD_TYPE MATCHES Debug)
   add_subdirectory(Tests)
endif()
