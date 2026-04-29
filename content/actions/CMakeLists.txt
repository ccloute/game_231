add_library(actions
  rest.h rest.cpp
        use.cpp
        use.h
)

target_include_directories(actions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(actions PUBLIC engine events)
