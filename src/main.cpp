#include "window.h"

int main(int, char**)
{
    Window w; 
    w.setupSDL();
    w.setupGLSL();
    w.setupWindow();
    w.setupImGui();
    w.setupImGuiStyle();
    w.render();
    return 0;
}
