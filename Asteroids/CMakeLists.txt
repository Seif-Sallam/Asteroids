cmake_minimum_required(VERSION 3.16)

FILE(GLOB SOURCES ./src/*.cpp)

FILE(GLOB HEADERS ./headers/*.h)

add_executable(
    Asteroids
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(
    Asteroids
    PRIVATE
    ./headers/
)
find_library(OPENAL_LIBRARY NAMES OpenAL32)

target_link_libraries(Asteroids
    PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Copy OpenAL library to the output directory
file(COPY ${OPENAL_LIBRARY} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(Asteroids
        PRIVATE
        RSC_DIR="${CMAKE_CURRENT_SOURCE_DIR}/rsc/"
    )
else()
    target_compile_definitions(Asteroids
        PRIVATE
        RSC_DIR="./rsc/"
    )

    set_target_properties(Asteroids PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(Asteroids PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
    if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
        file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
    endif()

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/rsc/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rsc/)
endif()
