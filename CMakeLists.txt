project(cpp-toolbox)

cmake_minimum_required(VERSION 2.8.11)

find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(JPEG REQUIRED)

#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++14 -fPIC -Wall -Wno-unknown-pragmas -Wno-strict-aliasing")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/plog/include/" ${Boost_INCLUDE_DIRS}) 
message(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ ${Boost_INCLUDE_DIRS}) 
link_directories(${BOOST_LIBRARY_DIRS})

add_library(toolbox
  cnpy.cc
  tokenize.cc
  kb.cc
  gettid.cc
  gettime.cc
  colormap.cc
  Program.cc
  ProgramOptions.cc
  libjpeg.cc)
  
#add_subdirectory(3rdparty/fmt)

target_link_libraries(toolbox fmt jpeg png jsoncpp ${Boost_LIBRARIES})
