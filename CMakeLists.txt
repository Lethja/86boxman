option(APP_IMAGE "Build portable App Image on Linux" ON)

if (APPLE OR (LINUX AND APP_IMAGE))
    cmake_minimum_required(VERSION 3.21)
else ()
    cmake_minimum_required(VERSION 3.1...3.21)
endif ()

set(PROJECT_NAME "86boxman")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")

project(${PROJECT_NAME} VERSION 0.01)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
        Core
        Gui
        Network
        Widgets
)

if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
            Core
            Core5Compat
            Gui
            Network
            Widgets
    )
else ()
    find_package(Qt5 REQUIRED COMPONENTS
            Core
            Gui
            Network
            Widgets
    )
endif ()

add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        src/main.cpp
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
        src/CreateMachine.ui
)

target_link_libraries(${PROJECT_NAME}
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Gui
        Qt${QT_VERSION_MAJOR}::Network
        Qt${QT_VERSION_MAJOR}::Widgets
)

if (QT_VERSION_MAJOR GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Core5Compat)
endif ()

if (APPLE)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/../.."
            -always-overwrite
            COMMENT "Running macdeployqt..."
    )
endif ()

if (WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_STRIP} ${PROJECT_NAME}.exe)
endif ()

if (LINUX AND APP_IMAGE)
    install(TARGETS ${PROJECT_NAME} RUNTIME_DEPENDENCIES )
else ()
    install(TARGETS ${PROJECT_NAME} BUNDLE DESTINATION . )
endif ()