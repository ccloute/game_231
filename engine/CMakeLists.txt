project(engine)

include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.4.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
)

FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
        OVERRIDE_FIND_PACKAGE
)
# disable formats we don't use to make the build faster and smaller
set(SDLMIXER_MIDI_NATIVE OFF)
set(SDLMIXER_GME OFF)
set(SDLMIXER_WAVPACK OFF)
set(SDLMIXER_MOD OFF)
set(SDLMIXER_OPUS OFF)

# FetchContent_Declare(
#         SDL_image
#         GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#         GIT_TAG release-3.4.0
#         GIT_SHALLOW TRUE
#         GIT_PROGRESS TRUE
#         OVERRIDE_FIND_PACKAGE
# )
# # disable formats we don't use to make the build faster and smaller
# set(SDLIMAGE_AVIF OFF)
# set(SDLIMAGE_BMP OFF)
# set(SDLIMAGE_JPEG OFF)
# set(SDLIMAGE_WEBP OFF)

# FetchContent_MakeAvailable(SDL SDL_mixer SDL_image)
FetchContent_MakeAvailable(SDL SDL_mixer)

add_subdirectory(util)
add_subdirectory(graphics)
add_subdirectory(dungeon)
add_subdirectory(input)
add_subdirectory(audio)

add_library(engine
  action.h action.cpp
  entity.h entity.cpp
  entities.h entities.cpp
  camera.h camera.cpp
  engine.h engine.cpp
  event.h event.cpp
  events.h events.cpp
  settings.h settings.cpp
  item.h item.cpp
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine PUBLIC util graphics dungeon input audio)

