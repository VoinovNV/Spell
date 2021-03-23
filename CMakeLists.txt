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
    src/dict_settings.hpp
    src/dict_settings.ui
    src/m_dict.cpp
    src/m_dict.hpp
)
target_link_libraries(Spell PRIVATE
    Qt5::Widgets
    Nuspell::nuspell
 )
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dic/personal.aff
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/dic/personal.dic
DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_compile_definitions(CUSTOM_DICTIONARY_PATH="${CMAKE_CURRENT_BINARY_DIR}/personal")

