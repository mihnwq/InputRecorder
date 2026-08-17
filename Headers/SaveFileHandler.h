//
// Created by Mihnwq on 8/12/2026.
//

#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class SaveFileHandler
{
    static inline  string selectedSaveFile = "";

    public:
    SaveFileHandler();
    
    void CreateSaveFile(string fileName) const;

    void DeleteSaveFile() const;

    static void SetSelectedSaveFile(string fileName)
    {
        selectedSaveFile = fileName;
    };

    static string GetSelectedSaveFile()
    {
        return selectedSaveFile;
    }

    ~SaveFileHandler();
};
