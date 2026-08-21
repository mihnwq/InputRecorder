#include "../Headers/UIManager.h"

#include <filesystem>

#include "imgui.h"
#include "../Headers/BackendTasks.h"
#include "../Headers/SaveFileHandler.h"

using namespace std;

void UIManager::RenderUI()
{
    if (currentPage == Page::Main)
    {
        RenderMainUI();
    }
    else if (currentPage == Page::Sequence)
    {
        RenderSequenceUI();
    }
}

void UIManager::CreateSaveFilePopUp() const
{
    if (ImGui::BeginPopupModal("Create New Save", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {

        static char saveName[256] = "";

        ImGui::Text("Enter save file name:");

        ImGui::InputText("##SaveName", saveName, sizeof(saveName));

        if (ImGui::Button("Create"))
        {
            if (saveName[0] != '\0')
            {
                if (currentPage == Page::Main)
                     BackendTasks::CreateSaveFile(saveName);
                else
                    BackendTasks::CreateSaaveFileSequence(saveName);

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
}

void UIManager::DisplaySaveFilesMenu(std::filesystem::path saveDirectory)
{
    ImGui::SameLine();

    ImGui::BeginGroup();

    ImGui::Text("Your save files");

    ImGui::BeginChild(
        "SaveFileList",
        ImVec2(300, 300),
        true
    );

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

            bool isSelected = (selectedSaveFileMain == fileName);

            if (ImGui::Selectable(displayName.c_str(), isSelected))
            {
                selectedSaveFileMain = fileName;

                SaveFileHandler::SetSelectedSaveFile(selectedSaveFileMain);
            }
        }
    }

    ImGui::EndChild();

    ImGui::EndGroup();

}

void UIManager::RenderSequenceUI()
{
    ImGui::SetNextWindowSize(ImVec2(1000, 1000), ImGuiCond_FirstUseEver);
    ImGui::Begin("System Sequences");

    if (ImGui::Button("Back"))
    {
        currentPage = Page::Main;
        SaveFileHandler::SetSelectedSaveFile("");
    }

    if (ImGui::Button("Record"))
    {
        BackendTasks::ExecuteSequenceRecording();
    }

    if (ImGui::Button("Terminnate"))
    {
        BackendTasks::TerminateAllTasks();
    }

    if (ImGui::Button("Check for sequence activation"))
    {
        BackendTasks::CheckSequenceExecution();
    }

    if (ImGui::Button("Create new save file")) {
        ImGui::OpenPopup("Create New Save");
    }

    CreateSaveFilePopUp();

    DisplaySaveFilesMenu("D:/MovementRecorder/SaveFiles/ExistingSaveFilesSequences/");

    ImGui::End();
}

void UIManager::RenderMainUI()
{

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

    if (ImGui::Button("Open Sequence Page"))
    {
        currentPage = Page::Sequence;
        SaveFileHandler::SetSelectedSaveFile("");
    }

    CreateSaveFilePopUp();

    DisplaySaveFilesMenu("D:/MovementRecorder/SaveFiles/ExistingSaveFiles/");


    ImGui::End();
}

