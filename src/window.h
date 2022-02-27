#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>

#if defined(IMGUI_IMPL_OPENGL_ES2)
    #include <SDL_opengles2.h>
#else
    #include <SDL_opengl.h>
#endif

#include <iostream>
#include <chrono>
#include <thread>

#include "fonts/JetBrainsMono.cpp"

#include "editor.h"

class Window {
public:
    Window();
    ~Window();
// private:
    std::string glslVersion; 
    SDL_GLContext glContext; 
    
    SDL_Window* window;
    ImGuiIO* io;
    Editor* currentEditor;

    bool setupSDL();
    void setupGLSL(); 
    void setupWindow(); 
    void setupImGui();
    void setupImGuiStyle(); 

    void render();
};