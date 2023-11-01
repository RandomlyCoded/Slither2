cmake_minimum_required(VERSION 3.14)

project(Slither2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(Qt6 COMPONENTS Core Quick Test REQUIRED)

#add_subdirectory(tests)

add_subdirectory(Botdefs)

qt_add_executable(
    Slither2 WIN32
    assets/assets.qrc
    assets/pendants.qrc

    qml/qml.qrc

    src/leaderboard.cpp src/leaderboard.h
    src/main.cpp
    src/playground.cpp src/playground.h
    src/snake.cpp src/snake.h
    src/playground_savestreams.cpp
)

target_compile_definitions(Slither2
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Slither2
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
  PRIVATE Botdefs
)
