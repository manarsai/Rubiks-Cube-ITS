cmake_minimum_required(VERSION 3.16)
project(RUBIKSCUBE_ITS)

set(CMAKE_CXX_STANDARD 17)
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
# SOURCE FILES (AUTO DISCOVERY)
# =========================
file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/app/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_SOURCE_DIR}/app/*.h
    ${CMAKE_SOURCE_DIR}/src/*.h
)

# =========================
# EXECUTABLE
# =========================
add_executable(RUBIKSCUBE_ITS
    ${SOURCES}
    ${HEADERS} "src/core/domain/Colour.h"  "src/core/domain/FaceDirections.h" "src/core/domain/Types.h" "src/core/domain/CubeUtils.h")

# =========================
# LINK LIBRARIES
# =========================
target_link_libraries(RUBIKSCUBE_ITS PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
    SQLite::SQLite3
    ${OpenCV_LIBS}
)

# =========================
# INCLUDE DIRECTORIES
# =========================
target_include_directories(RUBIKSCUBE_ITS PRIVATE
    ${CMAKE_SOURCE_DIR}
)