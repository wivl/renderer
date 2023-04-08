cmake_minimum_required(VERSION 3.10)

project(renderer)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

include_directories("${PROJECT_SOURCE_DIR}/dependencies")

file(GLOB SRC_FILES 
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.c"
	"${PROJECT_SOURCE_DIR}/src/*.hpp"
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/*.cpp"
    "${PROJECT_SOURCE_DIR}/dependencies/*.hpp"
    "${PROJECT_SOURCE_DIR}/dependencies/*.c"
    "${PROJECT_SOURCE_DIR}/dependencies/*.h"
    )

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})


# target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)
# if you are using neovim with coc-clangd, than you should generate a file using 
# ```
# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
# ```
# in your build directery to avoid error message by coc-clangd.
