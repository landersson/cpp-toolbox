cmake_minimum_required(VERSION 3.10)

add_library(toolbox
    cnpy.cc
    tokenize.cc
    kb.cc
    gettid.cc
    gettime.cc
    colormap.cc
    Program.cc
    ProgramOptions.cc
    libjpeg.cc
    ${CMAKE_SOURCE_DIR}/extern/jsoncpp.cpp)


target_include_directories(toolbox PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_features(toolbox PUBLIC cxx_std_14)
set_target_properties(toolbox PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(toolbox PUBLIC
    fmt 
    jpeg 
    png 
    plog
    ${Boost_LIBRARIES})

