cmake_minimum_required(VERSION 3.16)
project(RUBIKSCUBE_ITS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

find_package(OpenGL REQUIRED)

# enable qt tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# opencv
find_package(OpenCV REQUIRED)

#sqlite

#src files
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/app/*.cpp"
    "${CMAKE_SOURCE_DIR}/src/vision/*.cpp"
)

add_executable(RUBIKSCUBE_ITS ${SOURCES} "app/main.cpp"     "src/vision/vision.cpp" "src/vision/vision.h"  "src/ui/MainWindow.cpp"  "src/core/domain/cubeState.h" "src/core/domain/cubeScanner.h" "src/ui/cameraWidget.cpp" "src/ui/cameraWidget.h" "src/ui/MainWindow.h"  "src/core/domain/Cube.cpp" "src/core/domain/Cube.h" "src/core/domain/Moves.h" "src/core/domain/Moves.cpp" "src/core/domain/Solver.h" "src/core/domain/Solver.cpp" "src/visual/cubeView.h" "src/visual/cubeView.cpp" "src/ui/Camerascreen.h" "src/ui/Camerascreen.cpp" "src/ui/CubeNet.h" "src/ui/CubeNet.cpp")

#link libraries
target_link_libraries(RUBIKSCUBE_ITS PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    OpenGL::GL
  
    ${OpenCV_LIBS}
)
