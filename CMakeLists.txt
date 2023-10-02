cmake_minimum_required(VERSION 3.26)
project(86boxman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(86boxman main.cpp
        boxman.cpp
        boxman.h
        MainWindow.ui
        BoxManSettings.cpp
        BoxManSettings.h
        BoxManSettings.ui)
target_link_libraries(86boxman
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

