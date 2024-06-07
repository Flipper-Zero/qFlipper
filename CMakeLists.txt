cmake_minimum_required(VERSION 3.16)
project("qFlipper" C CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(FindCCache)
include(VersionInfo)
include(CompileOptions)
include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist")
    message(STATUS "Install Prefix defaulted to: ${CMAKE_INSTALL_PREFIX}")
else()
    message(STATUS "Install Prefix set to: ${CMAKE_INSTALL_PREFIX}")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Core5Compat Network SerialPort REQUIRED CONFIG)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(libusb REQUIRED IMPORTED_TARGET "libusb-1.0")
    pkg_check_modules(zlib REQUIRED IMPORTED_TARGET "zlib")
endif()

add_subdirectory("dfu")
add_subdirectory("backend")
add_subdirectory("plugins")
add_subdirectory("cli")
