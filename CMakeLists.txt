cmake_minimum_required(VERSION 3.22.1)
project(Minesweeper)

set (CMAKE_CXX_STANDARD 23)
set (CXX_STANDARD 23)
set(CMAKE_INCLUDE_CURRENT_DIR YES)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} `sdl2-config --cflags --libs` -lSDL2 -lSDL2_mixer -lSDL2_image -lSDL2_ttf")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Multimedia)
find_package(SDL2 REQUIRED)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

add_subdirectory("libs/SDL_image")

qt_wrap_cpp(MOC_Files
  include/SDLWidget.hpp
  include/MinesweeperWidget.hpp
)

add_executable(Minesweeper ${SRC_FILES} ${INCLUDE_FILE} ${MOC_Files})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

target_link_libraries( Minesweeper PRIVATE Qt6::Core Qt6::Widgets Qt6::Multimedia)
target_link_libraries( Minesweeper PRIVATE ${SDL2_LIBRARIES} )