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

           // cout << "Task 2 finished successfully!\n";

            delete mouse;

            taskState = IDLE;
        }).detach();

        thread([saveFileIndex]()
        {
            KeyboardAction* keyboard = new KeyboardAction();

            keyboard->ReadFile("D:/MovementRecorder/SaveFiles/Keyboard/KeyboardSaveFile" + saveFileIndex + ".txt");



            keyboard->DoAction();

           // cout << "Keyboard finished successfully!\n";

            delete keyboard;
        }).detach();
    }

    static void TerminateAllTasks()
    {
       // cout << SaveFileHandler::GetSelectedSaveFile() << endl;

        if (taskState != RECORDING) return;

        shouldTerminate = true;
    }

    static void CreateSaveFile(const string& fileName)
    {
       thread([fileName]()
       {

           SaveFileHandler saveFileHandler;

           saveFileHandler.CreateSaveFile(fileName);

       }).detach();
    }
};