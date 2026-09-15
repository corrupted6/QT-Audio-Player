cmake_minimum_required(VERSION 3.16)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# These three lines are the magic that replaces manual moc-ing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_FILES main.cpp)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main PRIVATE Qt6::Widgets Qt6::Multimedia)