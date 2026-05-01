cmake_minimum_required(VERSION 3.16)
project(RUBIKSCUBE_ITS)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# QT SETUP
# =========================
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# =========================
# EXTERNAL LIBRARIES
# =========================
find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(SQLite3 REQUIRED)

# =========================
# SOURCES
# =========================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/app/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(RUBIKSCUBE_ITS ${SOURCES})

target_link_libraries(RUBIKSCUBE_ITS PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
    ${OpenCV_LIBS}
    SQLite::SQLite3
)

target_include_directories(RUBIKSCUBE_ITS PRIVATE
    ${CMAKE_SOURCE_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# TESTING (THIS WAS MISSING)
# =========================
enable_testing()

add_executable(unit_tests
    tests/testing.cpp
)

add_test(NAME UnitTests COMMAND unit_tests)