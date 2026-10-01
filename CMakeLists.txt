cmake_minimum_required(VERSION 3.14)
project(yzaudiolib LANGUAGES CXX)

add_library(yzaudiolib INTERFACE)
add_library(yzaudiolib::yzaudiolib ALIAS yzaudiolib)

target_include_directories(yzaudiolib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)