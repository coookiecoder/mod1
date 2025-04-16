cmake_minimum_required(VERSION 3.28)

project(mod1 LANGUAGES CXX)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

include(FetchContent)
FetchContent_Declare(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 3.0.0 GIT_SHALLOW ON EXCLUDE_FROM_ALL SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(mod1 main.cpp source/Parser.cpp include/Parser.hpp source/Interpolater.cpp include/Interpolater.hpp)
include_directories(include)
target_link_libraries(mod1 PRIVATE SFML::Graphics)
