#pragma once
#include <iostream>
#include <cstring>
#include <filesystem>

class UIManager
{
    void RenderMainUI();
    void RenderSequenceUI();

    void CreateSaveFilePopUp() const;
    void DisplaySaveFilesMenu(std::filesystem::path saveDirectory);

    enum class Page
    {
        Main,
        Sequence
    };
    Page currentPage = Page::Main;

    std::string selectedSaveFileMain;
    std::string selectedSaveFileSequence;
public:
    void RenderUI();
};

//hello