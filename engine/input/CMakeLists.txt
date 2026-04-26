find_package(SDL REQUIRED)

add_library(input
  input.h input.cpp
)


target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PRIVATE ${SDL_INCLUDE_DIRS})
target_link_libraries(input PRIVATE SDL3::SDL3)

