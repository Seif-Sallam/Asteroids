cmake_minimum_required(VERSION 3.16)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(CPM)

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

CPMAddPackage(
    NAME SFML
    GIT_REPOSITORY git@github.com:Seif-Sallam/SFML.git
    GIT_TAG 2.5.x
)
project(Astroids LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Astroids)