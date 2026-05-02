add_library(events
  display_damage.h display_damage.cpp
  updatefov.h updatefov.cpp
        animation.cpp
        animation.h
        hit.h hit.cpp
        die.h die.cpp
        thrust.h thrust.cpp
)

target_include_directories(events PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(events PUBLIC engine)
