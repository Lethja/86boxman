cmake_minimum_required(VERSION 3.1)
project(86boxman)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(86boxman src/main.cpp
        src/boxman.cpp
        src/boxman.h
        src/MainWindow.ui
        src/BoxManSettings.cpp
        src/BoxManSettings.h
        src/BoxManSettingsUi.cpp
        src/BoxManSettingsUi.h
        src/BoxManSettingsUi.ui
        src/CreateMachine.cpp
        src/CreateMachine.h
        src/CreateMachine.ui)

target_link_libraries(86boxman
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

install(TARGETS 86boxman)
install(FILES res/86boxman.desktop DESTINATION share/applications)

