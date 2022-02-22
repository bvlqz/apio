#include "imgui.h"
#include <fstream>

class Editor {
public:
    Editor();
    // ~Editor();
    void draw();

    void setFileName();
    
    bool setPath(const char * path);

    bool open();
    bool save();
private:
    char pathBuf[1024] = ""; 
    char tabNameBuf[256] = "New";
    char contentBuf[1024 * 16] = "";
};