cmake_minimum_required(VERSION 3.16)
project(ollvm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS off)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_CURRENT_LIST_DIR}/cmake)
include(OLLVMStandalone)

add_subdirectory(src)
add_subdirectory(tools)

