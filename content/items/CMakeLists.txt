add_library(items
  bite.h bite.cpp
        spear.cpp
        spear.h
)

target_include_directories(items PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(items PUBLIC engine events)
