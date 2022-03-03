#include "imgui.h"
#include <fstream>
#include <iostream>

#include <sys/stat.h>

class Editor {
public:
    Editor();
    // ~Editor();
    void draw();
    void drawDebug();

    void drawPermissions(); 
    void drawCurrentFile(); 

    void setFileName();
    
    bool setPath(const char * path);

    bool open();
    bool save();
private:
    char pathBuf[1024] = ""; 
    char tabNameBuf[256] = "New";
    char contentBuf[1024 * 16] = "";
    char permissions[10] = "---------";
};