cmake_minimum_required(VERSION 3.1...3.4)

set(PROJECT_NAME "86boxman")
project(${PROJECT_NAME} VERSION 0.01)

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

add_executable(${PROJECT_NAME} src/main.cpp
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

target_link_libraries(${PROJECT_NAME}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${PROJECT_NAME}.exe)
else ()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${PROJECT_NAME})
endif ()

install(TARGETS ${PROJECT_NAME})

if (NOT WIN32)
    install(FILES res/86boxman.desktop DESTINATION share/applications)
endif ()
