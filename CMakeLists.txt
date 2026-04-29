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
# SOURCE FILES (SAFE VERSION)
# =========================
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/app/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# ⚠️ DO NOT include headers in the executable
# They are not compiled and only confuse the build system.

# =========================
# EXECUTABLE
# =========================
add_executable(RUBIKSCUBE_ITS
    ${SOURCES}
)

# =========================
# LINK LIBRARIES
# =========================
target_link_libraries(RUBIKSCUBE_ITS PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
    ${OpenCV_LIBS}
    SQLite::SQLite3
)

# =========================
# INCLUDE DIRECTORIES
# =========================
target_include_directories(RUBIKSCUBE_ITS PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# =========================
# CLEAN BUILD SAFETY FLAG
# =========================
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)