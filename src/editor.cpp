#include "editor.h"

Editor::Editor()
{

}

void Editor::draw()
{
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    static ImGuiWindowFlags windowFlags = 0;
    static bool userWantsEditPath = false;

    windowFlags |= ImGuiWindowFlags_NoCollapse;
    windowFlags |= ImGuiWindowFlags_MenuBar;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin("Text Editor", NULL, windowFlags); 

    ImGuiIO& io = ImGui::GetIO();
    if (io.KeyCtrl || io.KeySuper)
    {
        if (ImGui::IsKeyDown(ImGuiKey_S)) save();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl+S")) {save();}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
    ImGui::InputTextMultiline("##source", contentBuf, IM_ARRAYSIZE(contentBuf), ImVec2(-FLT_MIN, -FLT_MIN - ImGui::GetTextLineHeight() - 10), flags);

    ImGui::Text("%s", permissions); 

    if (userWantsEditPath)
    {
        ImGui::InputText("##FileName", pathBuf, IM_ARRAYSIZE(pathBuf));
        ImGui::SameLine();
        if (ImGui::Button("OK"))
        {
            userWantsEditPath = false;
        }
    } else {
        ImGui::Text("%s", pathBuf);
        if (ImGui::IsMouseDoubleClicked(0))
        {
            userWantsEditPath = true;
        }
    }

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