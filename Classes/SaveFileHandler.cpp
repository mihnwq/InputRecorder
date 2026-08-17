//
// Created by Mihnwq on 8/12/2026.
//

#include "../Headers/SaveFileHandler.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

SaveFileHandler::SaveFileHandler() {}

void SaveFileHandler::CreateSaveFile(string fileName) const
{
    int currentSaveFileNumber = 0;

    {
        ifstream file("D:/MovementRecorder/SaveFiles/SaveFilesNumber/FileCurrentNumber.txt");
        file >> currentSaveFileNumber;
    }

    {

        string path = "D:/MovementRecorder/SaveFiles/ExistingSaveFiles/" + fileName + ".txt";
        ofstream file(path, ios::trunc | ios::in | ios::out);


        file << currentSaveFileNumber;
    }


    {
        ofstream file("D:/MovementRecorder/SaveFiles/SaveFilesNumber/FileCurrentNumber.txt", ios::trunc);
        file << ++currentSaveFileNumber;
    }

}

SaveFileHandler::~SaveFileHandler(){}
