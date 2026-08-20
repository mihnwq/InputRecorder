#pragma once
#include <iostream>
#include <cstring>

class UIManager
{
    void RenderMainUI();
    void RenderSequenceUI();

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