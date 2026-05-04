add_library(actions
  rest.h rest.cpp
        use.cpp
        use.h
        move.h move.cpp
        opendoor.h opendoor.cpp
        closedoor.h closedoor.cpp
        wander.h wander.cpp
        attack.h attack.cpp
        dart_path.h dart_path.cpp
)

target_include_directories(actions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(actions PUBLIC engine events)
