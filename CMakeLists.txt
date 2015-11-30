cmake_minimum_required (VERSION 2.6)
project (Lights)
add_executable(color-test color-test.cpp color.cpp)

set(LIGHT_PARTS
Parts/HotPlate.cpp
Parts/Screen.cpp
Parts/GameOfLife.cpp
Parts/AStar.cpp
Parts/AStarTest.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lights main.cpp color.cpp ${LIGHT_PARTS})