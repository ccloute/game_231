find_package(SDL REQUIRED)

# Classes and functions for loading images and drawing to the screen
add_library(graphics
  animatedsprite.h animatedsprite.cpp
  graphics.h graphics.cpp
  sprite.h sprite.cpp
)

target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL_INCLUDE_DIRS})
target_link_libraries(graphics PUBLIC util SDL3::SDL3)
