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
    windowFlags |= ImGuiWindowFlags_NoResize;
    windowFlags |= ImGuiWindowFlags_NoDecoration;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    
    ImVec2 editorSize = viewport->Size;
    editorSize[1] = editorSize[1] - 18;

    ImVec2 editorPosition = viewport->Pos;
    editorPosition[1] = editorPosition[1] + 18;

    ImGui::SetNextWindowPos(editorPosition);
    ImGui::SetNextWindowSize(editorSize);

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

    struct stat st;

    if(stat(pathBuf, &st) == 0){
        mode_t permission = st.st_mode;
        
        permissions[0] = (permission & S_IRUSR) ? 'r' : '-';
        permissions[1] = (permission & S_IWUSR) ? 'w' : '-';
        permissions[2] = (permission & S_IXUSR) ? 'x' : '-';
        permissions[3] = (permission & S_IRGRP) ? 'r' : '-';
        permissions[4] = (permission & S_IWGRP) ? 'w' : '-';
        permissions[5] = (permission & S_IXGRP) ? 'x' : '-';
        permissions[6] = (permission & S_IROTH) ? 'r' : '-';
        permissions[7] = (permission & S_IWOTH) ? 'w' : '-';
        permissions[8] = (permission & S_IXOTH) ? 'x' : '-';
    } else {
        // error
    }


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