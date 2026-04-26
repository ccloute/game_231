add_library(dungeon
  builder.h builder.cpp
  decorator.h decorator.cpp
  door.h door.cpp
  fog.h fog.cpp
  fov.h fov.cpp
  dungeon.h dungeon.cpp
  pathfinding.h pathfinding.cpp
  room.h room.cpp
  tile.h tile.cpp
)

target_include_directories(dungeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(dungeon PUBLIC util graphics)
