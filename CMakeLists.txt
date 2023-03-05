cmake_minimum_required(VERSION 3.14)

project(Slither2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(CMAKE_PREFIX_PATH C:/Users/Marc/Programmieren/Libs/NeuralNet)

find_package(NeuralNet REQUIRED)

add_library(Botdefs)

target_link_libraries(
    Botdefs
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
    PRIVATE NeuralNet)

target_sources(
    Botdefs PRIVATE
    Bots
    bot.h bot.cpp

    aibot.h aibot.cpp
    eating_bot.h eating_bot.cpp
    followmouse_bot.h followmouse_bot.cpp
    killing_bot.h killing_bot.cpp
    stupid_bot.h stupid_bot.cpp

#    NeuralNets/netBase.h NeuralNets/netBase.cpp
#    NeuralNets/net1.h NeuralNets/net1.cpp
#    NeuralNets/net2.h NeuralNets/net2.cpp
)

set(PROJECT_SOURCES
    assets.qrc
    leaderboard.cpp leaderboard.h
    main.cpp
    pendants.qrc
    playground.cpp playground.h
    qml.qrc
    snake.cpp snake.h
    playground_savestreams.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Slither2 WIN32
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Slither2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Slither2
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(Slither2
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(Slither2
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick
  PRIVATE Botdefs
  PRIVATE NeuralNet)
