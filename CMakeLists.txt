cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(SOURCES
    main.c
    stack/stack.c
    aux/aux.c
)

add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/stack)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/aux)
target_compile_options(main PRIVATE -Wall -Wextra -pedantic -lm -fsanitize=address)
target_link_options(main PRIVATE -lm -fsanitize=address)