#include "../Headers/UIManager.h"

#include <filesystem>

#include "imgui.h"
#include "../Headers/BackendTasks.h"
#include "../Headers/SaveFileHandler.h"

using namespace std;

void UIManager::RenderUI() {

    ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_FirstUseEver);

    ImGui::Begin("Control Panel");
    ImGui::Text("Application Status: Running");
    
    // Triggering tasks based on button clicks
    if (ImGui::Button("Record"))
    {
        BackendTasks::ExecuteRecord();
    }
    
    if (ImGui::Button("Replay")) {
        BackendTasks::ExecuteReplay();
    }

    if (ImGui::Button("Terminate")) {
        BackendTasks::TerminateAllTasks();
    }

    if (ImGui::Button("CreateNewSave"))
    {

        ImGui::OpenPopup("Create New Save");
    }

    if (ImGui::BeginPopupModal("Create New Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {

        static char saveName[256] = "";

        ImGui::Text("Enter save file name:");

        ImGui::InputText("##SaveName", saveName, sizeof(saveName));

        if (ImGui::Button("Create"))
        {
            if (saveName[0] != '\0')
            {
                BackendTasks::CreateSaveFile(saveName);

                saveName[0] = '\0';

                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            saveName[0] = '\0';
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    // Move to the right side
    ImGui::SameLine();

    ImGui::BeginGroup();

    ImGui::Text("Your save files");

    ImGui::BeginChild(
        "SaveFileList",
        ImVec2(300, 300),
        true
    );

    filesystem::path saveDirectory = "D:/MovementRecorder/SaveFiles/ExistingSaveFiles/";

    if (filesystem::exists(saveDirectory) &&
        filesystem::is_directory(saveDirectory))
    {


        for (const auto& file : filesystem::directory_iterator(saveDirectory))
        {
            if (!file.is_regular_file())
                continue;

            if (file.path().extension() != ".txt")
                continue;

            string fileName = file.path().filename().string();
            string displayName = file.path().stem().string();

           // cout<<displayName<<endl;

            bool isSelected = (selectedSaveFile == fileName);

            if (ImGui::Selectable(displayName.c_str(), isSelected))
            {
                selectedSaveFile = fileName;

                SaveFileHandler::SetSelectedSaveFile(selectedSaveFile);
            }
        }
    }

    ImGui::EndChild();

    ImGui::EndGroup();

    ImGui::End();
}

