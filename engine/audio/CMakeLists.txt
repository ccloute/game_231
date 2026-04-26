find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

add_library(audio
  audio.h audio.cpp
)

target_include_directories(audio PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL3_INCLUDE_DIRS} ${SDL3_MIXER_INCLUDE_DIRS})
target_link_libraries(audio PUBLIC SDL3::SDL3 SDL3_mixer::SDL3_mixer)
