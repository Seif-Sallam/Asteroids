cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCES ./src/*.cpp)

FILE(GLOB HEADERS ./headers/*.h)

add_executable(
    Astroids
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(
    Astroids
    PRIVATE
    ./headers/
)

target_link_libraries(Astroids
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

target_compile_definitions(Astroids
    PRIVATE
    RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/"
)