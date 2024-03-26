cmake_minimum_required(VERSION 3.28)
project(GPUFluid)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/Cellar/glfw/3.4/include)
include_directories(include)

file(GLOB_RECURSE SRC1 "src/*.cpp")
file(GLOB_RECURSE SRC2 "src/*.c")
file(GLOB_RECURSE SRC3 "lib/*.cpp")
file(GLOB_RECURSE SRC4 "lib/*.c")
list(APPEND SRC ${SRC1})
list(APPEND SRC ${SRC2})
list(APPEND SRC ${SRC3})
list(APPEND SRC ${SRC4})

message(SRC="${SRC}")

add_executable(GPUFluid ${SRC})

target_link_libraries(GPUFluid /opt/homebrew/Cellar/glfw/3.4/lib/libglfw.dylib)