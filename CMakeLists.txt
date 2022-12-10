cmake_minimum_required(VERSION 3.25.1)
project(wordfeud VERSION 1.0.0)

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -Werror
)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

add_subdirectory(src)