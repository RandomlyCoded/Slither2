cmake_minimum_required(VERSION 3.14)

project(skinEditor2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Quick REQUIRED)

set(PROJECT_SOURCES
    _dataStreams.cpp
    editor.cpp editor.h
    main.cpp
    qml.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(skinEditor2
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(skinEditor2 SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(skinEditor2
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(skinEditor2
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(skinEditor2
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick)
