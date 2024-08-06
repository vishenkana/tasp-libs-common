cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/submodule/cmake")

include(DetectOs)
include(SetupEnvironment)

project(tasp-common LANGUAGES CXX)

include(SetupCompileOptions)
include(SetupHardening)

include_directories(include)

file(GLOB_RECURSE SOURCES
  ./src/*.cpp
)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

include(Version)

find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(YAML_CPP REQUIRED yaml-cpp)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        stdc++fs
    PRIVATE
        Threads::Threads
        yaml-cpp
        jsoncpp
)

include(SetupInstall)
