cmake_minimum_required(VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project(apio)

# Warnings
add_definitions("-Wall")
add_definitions("-Wextra")
add_definitions("-pedantic") 
add_definitions("-Wconversion") 

option(APIO_DEVELOPMENT_BUILD "Development Build" OFF)
if (APIO_DEVELOPMENT_BUILD)
    add_definitions(-DAPIO_DEVELOPMENT_BUILD)
endif()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(vendor/imgui)

add_executable(apio 
    src/main.cpp
    src/window.h
    src/window.cpp
    src/editor.h
    src/editor.cpp
    src/fonts/JetBrainsMono.cpp
    vendor/imgui/imconfig.h
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui.h
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_internal.h
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/imstb_rectpack.h
    vendor/imgui/imstb_textedit.h
    vendor/imgui/imstb_truetype.h
    vendor/imgui/imgui_impl_opengl3.cpp
    vendor/imgui/imgui_impl_opengl3.h
    vendor/imgui/imgui_impl_sdl.cpp
    vendor/imgui/imgui_tables.cpp
)

target_link_libraries(apio ${OPENGL_gl_LIBRARY} ${SDL2_LIBRARIES})