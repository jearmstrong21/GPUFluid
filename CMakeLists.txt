cmake_minimum_required(VERSION 3.28)
project(GPUFluid)

set(CMAKE_CXX_STANDARD 17)

file(REMOVE_RECURSE include/gen)

add_executable(copyfile copy_file.c)

# Add a custom command to use copy_file.c to generate this artifact
# Then add the generated header file as a direct dependency of src/main.cpp
function(copyfile dir name ext)
    add_custom_command(
            OUTPUT include/gen/${dir}_${name}_${ext}.h
            COMMAND copyfile ${dir}/${name}.${ext} ${dir}_${name}_${ext}.h
            DEPENDS ${dir}/${name}.${ext}
    )
    set_property(SOURCE src/main.cpp APPEND PROPERTY OBJECT_DEPENDS include/gen/${dir}_${name}_${ext}.h)
endfunction()

copyfile(shader quad vert)
copyfile(shader simple_fast_init frag)
copyfile(shader simple_fast_update frag)
copyfile(shader simple_fast_display frag)
copyfile(shader copy frag)

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

add_executable(GPUFluid ${SRC}
        src/Quad.cpp
        src/Quad.h
        src/Computation.cpp
        src/Computation.h
        src/ImageData.cpp
        src/ImageData.h
)

target_link_libraries(GPUFluid /opt/homebrew/Cellar/glfw/3.4/lib/libglfw.dylib)