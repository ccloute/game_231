add_library(items
  bite.h bite.cpp
        spear.cpp
        spear.h
        sword.h sword.cpp
        mace.h mace.cpp
        potion.h potion.cpp
        poison_dart.h poison_dart.cpp
)

target_include_directories(items PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(items PUBLIC engine events actions)
