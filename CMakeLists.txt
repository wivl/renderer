cmake_minimum_required(VERSION 3.10)

project(renderer)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_C_COMPILER "/usr/bin/clang")


file(GLOB SRC_FILES
	"${PROJECT_SOURCE_DIR}/*.h"
	"${PROJECT_SOURCE_DIR}/*.cpp"
	"${PROJECT_SOURCE_DIR}/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

# if you are using neovim with coc-clangd, than you should generate a file using 
# ```
# cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1
# ```
# in your build directery to avoid error message by coc-clangd.
