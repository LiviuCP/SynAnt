cmake_minimum_required(VERSION 3.14)

project(SynAntApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Application)
add_subdirectory(SystemFunctionality)

if (CMAKE_BUILD_TYPE MATCHES Debug)
   add_subdirectory(Tests)
endif()
