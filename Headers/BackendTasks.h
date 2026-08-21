#pragma once
#include <thread>
#include <iostream>
#include <chrono>

#include "KeyboardAction.h"
#include "MouseAction.h"
#include "SaveFileHandler.h"

using namespace std;

class BackendTasks
{
    static inline std::atomic<bool> shouldTerminate = false;

    static inline  enum TaskState
    {
        RECORDING,
        REPLAYING,
        CREATING_SAVEFILE,
        DELETING_SAVEFILE,
        IDLE
    } taskState = IDLE;

public:

    static void ExecuteRecord()
    {
        shouldTerminate = false;

        string currentFileName = SaveFileHandler::GetSelectedSaveFile();

        if (currentFileName.size() <= 1)
            return;

        string saveFileIndex;

        {
            ifstream file("D:/MovementRecorder/SaveFiles/ExistingSaveFiles/" + currentFileName);

            file >> saveFileIndex;
        }

        thread([saveFileIndex]()
        {
           MouseAction* mouse = new MouseAction();
           MouseHookHandler* hook = new MouseHookHandler();

            taskState = RECORDING;

            mouse->SetFile("D:/MovementRecorder/SaveFiles/Mouse/MouseSaveFile" + saveFileIndex + ".txt");

            mouse->RecordAction(hook,&shouldTerminate);


           // cout << "Task 1 finished successfully!\n";

            delete mouse;

            delete hook;

            taskState = IDLE;

        }).detach();

        thread([saveFileIndex]()
        {


            KeyboardAction* keyboard = new KeyboardAction();
            KeyboardHookHandler* hook = new KeyboardHookHandler();

            keyboard->SetFile("D:/MovementRecorder/SaveFiles/Keyboard/KeyboardSaveFile" + saveFileIndex + ".txt");

            keyboard->RecordAction(hook,&shouldTerminate);


            delete keyboard;
            delete hook;

           // cout << "Deletion 2 finished successfully!\n";
        }).detach();
    }

    static void ExecuteReplay()
    {
        if (taskState != IDLE) return;

        string saveFileIndex;

        string currentFileName = SaveFileHandler::GetSelectedSaveFile();

        if (currentFileName.size() <= 1)
            return;

        {
            ifstream file("D:/MovementRecorder/SaveFiles/ExistingSaveFiles/" + currentFileName);

            file >> saveFileIndex;
        }

        thread([saveFileIndex]() {

            taskState = REPLAYING;

            MouseAction* mouse = new MouseAction();

            mouse->ReadFile("D:/MovementRecorder/SaveFiles/Mouse/MouseSaveFile" + saveFileIndex + ".txt");

            ///You can change the speed however you want but the minimum recomended is 1000.
            mouse->DoAction(/*speed = n*/);

            delete mouse;

            taskState = IDLE;
        }).detach();

        thread([saveFileIndex]()
        {
            KeyboardAction* keyboard = new KeyboardAction();

            keyboard->ReadFile("D:/MovementRecorder/SaveFiles/Keyboard/KeyboardSaveFile" + saveFileIndex + ".txt");

            keyboard->DoAction();

            delete keyboard;
        }).detach();
    }

    static void ExecuteSequenceRecording()
    {
        string saveFileIndex;

        string currentFileName = SaveFileHandler::GetSelectedSaveFile();

        if (currentFileName.size() <= 1)
            return;

        {
            ifstream file("D:/MovementRecorder/SaveFiles/ExistingSaveFilesSequences/" + currentFileName);

            file >> saveFileIndex;
        }

        if (currentFileName.size() <= 1)
            return;

        std::thread([saveFileIndex]()
        {

            KeyboardAction* keyboard = new KeyboardAction();
            KeyboardHookHandler* hook = new KeyboardHookHandler();

            keyboard->SetFile("D:/MovementRecorder/SaveFiles/SequenceSaveFiles/SequenceSaveFile" + saveFileIndex + ".txt");

            keyboard->RecordSequence(hook,&shouldTerminate);

            delete keyboard;
            delete hook;

        }).detach();
    }

    static void CheckSequenceExecution()
    {
        string saveFileIndex;

        string currentFileName = SaveFileHandler::GetSelectedSaveFile();

        if (currentFileName.size() <= 1)
            return;

        {
            ifstream file("D:/MovementRecorder/SaveFiles/ExistingSaveFiles/" + currentFileName);

            file >> saveFileIndex;
        }

        std::thread([saveFileIndex]()
      {

          KeyboardAction* keyboard = new KeyboardAction();
          KeyboardHookHandler* hook = new KeyboardHookHandler();

          keyboard->ReadFile("D:/MovementRecorder/SaveFiles/SequenceSaveFiles/SequenceSaveFile" + saveFileIndex + ".txt");

          keyboard->CheckSequence(hook,&shouldTerminate);

          delete keyboard;
          delete hook;

      }).detach();
    }

    static void TerminateAllTasks()
    {
        if (taskState != RECORDING) return;

        shouldTerminate = true;
    }

    static void CreateSaveFile(const string& fileName)
    {
        SaveFileHandler saveFileHandler;

        saveFileHandler.CreateSaveFile(fileName);
    }

    static void CreateSaaveFileSequence(const string& fileName)
    {
        SaveFileHandler saveFileHandler;

        saveFileHandler.CreateSaveFileSequences(fileName);
    }
};