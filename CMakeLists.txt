cmake_minimum_required(VERSION 3.5)

project(Spell LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Nuspell REQUIRED )
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_executable(Spell
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/mainwindow.ui
    src/dict_settings.cpp
    src/dict_settings.h
    src/dict_settings.ui
)
target_link_libraries(Spell PRIVATE
    Qt5::Widgets
    Nuspell::nuspell
 )
