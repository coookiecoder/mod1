cmake_minimum_required(VERSION 3.28)

project(mod1 LANGUAGES CXX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.0 GIT_SHALLOW ON EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(mod1 main.cpp Parser.cpp Parser.hpp
        Interpolater.cpp
        Interpolater.hpp)
target_link_libraries(mod1 PRIVATE SFML::Graphics)
