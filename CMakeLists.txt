cmake_minimum_required(VERSION 3.10)
project(cpp-toolbox LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(FetchSubModules)
include(BuildType)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(JPEG REQUIRED)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/extern/plog/)
add_subdirectory(src)
