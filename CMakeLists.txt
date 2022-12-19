cmake_minimum_required(VERSION 3.25.1)
project(wordfeud VERSION 1.0.0)

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

add_executable(
    ${PROJECT_NAME}
    main.cpp
    resources/resources.qrc
)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
add_subdirectory(src)
target_link_libraries(
    wordfeud PRIVATE
    Qt5::Widgets
    Settings
    Game
)