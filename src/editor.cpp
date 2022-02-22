#include "editor.h"

Editor::Editor()
{

}

void Editor::draw()
{
    static bool show_text_editor = true; 
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

    ImGui::Begin("Text Editor", &show_text_editor); 
    ImGui::InputText("File Name", pathBuf, IM_ARRAYSIZE(pathBuf));
    // ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
    ImGui::InputTextMultiline("##source", contentBuf, IM_ARRAYSIZE(contentBuf), ImVec2(-FLT_MIN, -FLT_MIN - 100), flags);


    if (ImGui::Button("Save")) save();

    ImGui::End();
}

bool Editor::save()
{
    std::ofstream stream;
    stream.open(pathBuf);
    if( !stream ) return false;
    stream << contentBuf << std::endl; // write
    if( !stream ) return false; // test if write was succesful
    return true;
}

bool Editor::open()
{
    std::string line;
    std::ifstream stream;
    stream.open(pathBuf);
    if( !stream ) return false;


    std::string contents(
        (std::istreambuf_iterator<char>(stream)), 
        std::istreambuf_iterator<char>());

    strncpy(contentBuf, contents.c_str(), sizeof(contentBuf));

    return true;
}

bool Editor::setPath(const char * path)
{
    strncpy(pathBuf, path, sizeof(pathBuf));
    return true;
}