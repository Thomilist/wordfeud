cmake_minimum_required(VERSION 3.25.1)
project(wordfeud VERSION 0.6.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
)

if(MINGW)
    set(CMAKE_CXX_FLAGS "-mwindows")    
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icon/icon.rc")
    add_executable(
        ${PROJECT_NAME}
        main.cpp
        resources/resources.qrc
        resources/styles/breeze/breeze.qrc
        ${APP_ICON_RESOURCE_WINDOWS}
    )
else()
    add_executable(
        ${PROJECT_NAME}
        main.cpp
        resources/resources.qrc
        resources/styles/breeze/breeze.qrc
    )
endif()

find_package(Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_subdirectory(src)
target_link_libraries(
    wordfeud PRIVATE
    Qt5::Widgets
    Version
    Wordfeud
)