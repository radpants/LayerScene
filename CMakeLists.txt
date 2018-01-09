cmake_minimum_required(VERSION 3.9)
project(LayerScene)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    include_directories(./external)
    include_directories(C:/DevelopmentLibraries/glfw-3.2.1.bin.WIN32/include)
    include_directories(C:/DevelopmentLibraries/glad/include)
    include_directories(C:/DevelopmentLibraries/glm-0.9.8.5)
    include_directories(C:/DevelopmentLibraries/tweeny-master/include)
    link_directories(C:/DevelopmentLibraries/glfw-3.2.1.bin.WIN32/lib-mingw)
    link_libraries(-static-libgcc -static-libstdc++ glfw3 gdi32 opengl32)
endif(WIN32)

add_executable(LayerScene main.cpp external/nanovg.c external/glad.c Layer.cpp Scene.cpp)