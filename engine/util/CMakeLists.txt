# Useful functions and classes used throughout the engine
add_library(util
  grid.h
  randomness.h randomness.cpp
  timer.h timer.cpp
  vec.h vec.cpp
)

target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
