cmake_minimum_required(VERSION 3.14)

project(Slither2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

add_library(Botdefs STATIC)
target_link_libraries(Botdefs
    PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
target_sources(
    Botdefs PRIVATE
    Bots
    bot.h bot.cpp
    eating_bot.h eating_bot.cpp
    followmouse_bot.h followmouse_bot.cpp
    killing_bot.h killing_bot.cpp
    stupid_bot.h stupid_bot.cpp
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
  Botdefs)
