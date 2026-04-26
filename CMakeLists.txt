cmake_minimum_required(VERSION 3.20)

project(game)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-g -Wall -Wextra -pedantic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# On Windows, create dll's in the same directory as the game executable
if (WIN32)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

add_subdirectory(engine)
add_subdirectory(content)

add_executable(game main.cpp)
target_link_libraries(game PUBLIC engine entities)
