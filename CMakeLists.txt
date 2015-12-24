cmake_minimum_required (VERSION 2.6)
project (Lights)
add_executable(color-test color-test.cpp color.cpp)

set(LIGHT_PARTS
Parts/HotPlate.cpp
Parts/Screen.cpp
Parts/GameOfLife.cpp
Parts/AStar.cpp
Parts/AStarTest.cpp
Parts/Snake.cpp
Parts/SmallPT.cpp
Parts/CornellBox.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
# add_definitions(-std=c++11)
	SET(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

add_executable(lights main.cpp color.cpp ${LIGHT_PARTS})
set_property(TARGET lights PROPERTY CXX_STANDARD 11)
set_property(TARGET lights PROPERTY CXX_STANDARD_REQUIRED ON)
