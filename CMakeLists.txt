cmake_minimum_required(VERSION 3.16)
project(Minesweeper)

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR YES)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lSDL2_image")

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Multimedia)
find_package(SDL2 REQUIRED)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB INCLUDE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)

qt5_wrap_cpp(MOC_Files
  include/SDLWidget.hpp
  include/MinesweeperWidget.hpp
)

add_executable(Minesweeper ${SRC_FILES} ${INCLUDE_FILE} ${MOC_Files})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

target_link_libraries( Minesweeper PRIVATE Qt5::Core Qt5::Widgets Qt5::Multimedia)
target_link_libraries( Minesweeper PRIVATE ${SDL2_LIBRARIES} )
