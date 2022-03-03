#include "window.h"

Window::Window()
{
    currentEditor = new Editor;
    io = nullptr;
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    delete currentEditor;

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::setupSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

// Decide GL+GLSL versions for ImGui
void Window::setupGLSL()
{
    #if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glslVersion = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    #elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glslVersion = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    
    #else
    // GL 3.0 + GLSL 130
    glslVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    
    #endif
}

// Create window with graphics context
void Window::setupWindow()
{
    int windowWidth, windowHeight;
    
    #ifdef APIO_DEVELOPMENT_BUILD
        windowWidth = 1000;
        windowHeight = 600;
    #else
        windowWidth = 480;
        windowHeight = 240;
    #endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);


    window = SDL_CreateWindow("Apio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, window_flags);
    
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync
}

void Window::setupImGui()
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init(glslVersion.c_str());
}

void Window::setupImGuiStyle(){
    // #define DARKGREY ImVec4(0.16f, 0.17f, 0.20f, 1.00f)

    ImGuiStyle* style = &ImGui::GetStyle();

    // style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    style->Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.17f, 0.20f, 0.80f);
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.17f, 0.20f, 1.00f);

    io->Fonts->AddFontFromMemoryCompressedTTF(JetBrainsMono_compressed_data, JetBrainsMono_compressed_size, 13);


}

void Window::render()
{
    // Main loop
    bool done = false;
    bool windowIsMinimized = false; 

    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) done = true;

            if (event.type == SDL_WINDOWEVENT 
                && event.window.windowID == SDL_GetWindowID(window))
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_CLOSE:
                        done = true;
                        break;
                    case SDL_WINDOWEVENT_MINIMIZED:
                        windowIsMinimized = true;
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        windowIsMinimized = false;
                        break;
                }
            }

            if (event.type == SDL_DROPFILE) 
            {
                std::string path = event.drop.file;
                std::replace(path.begin(), path.end(), '\\', '/');

                size_t found = path.find_last_of("/\\");
                std::string filename = path.substr(found + 1);
                SDL_SetWindowTitle(window, filename.c_str());

                std::cout << "Dropped path: " << path << std::endl;
                currentEditor->setPath(path.c_str());
                currentEditor->open();

                SDL_free(event.drop.file);

            }
        }

        if (windowIsMinimized) {std::this_thread::sleep_for(std::chrono::milliseconds(100));}

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        



        if (ImGui::BeginMainMenuBar())
        {
            currentEditor->drawCurrentFile(); 
            currentEditor->drawPermissions();
            ImGui::EndMainMenuBar();
        }


        currentEditor->draw();
        #ifdef APIO_DEVELOPMENT_BUILD
        currentEditor->drawDebug();
        #endif

        // Rendering
        ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);
        ImGui::Render();
        glViewport(0, 0, (int)io->DisplaySize.x, (int)io->DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
}