//
// Created by Mihnwq on 8/16/2026.
//

#include "../Headers/KeyboardAction.h"

#include <iostream>

void KeyboardAction::SetFile(std::string fileName)
{
    fileOUT = new std::fstream(fileName.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
}

void KeyboardAction::ReadFile(std::string fileName)
{
    fileIN = new std::fstream(fileName.c_str(), std::ios::in | std::ios::out);
    fileIN->seekg(0);
}

void KeyboardAction::PressKey(WORD keyCode) const
{
    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;


    SendInput(1, &input, sizeof(INPUT));

    input.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}

void KeyboardAction::RecordAction(KeyboardHookHandler *keyboardHookHandler, std::atomic<bool>* shoulldTerminate)
{

    if (!fileOUT || !keyboardHookHandler) return;

   // std::cout << "Hello";

    stopwatch.Start();

    while (!shoulldTerminate->load())
    {
        int currentKey = keyboardHookHandler->GetLastKey();


        if (currentKey)
        {
            double time = stopwatch.Stop();
            *fileOUT << time << std::endl;
            std::cout << time << std::endl;

            if (keyboardHookHandler->IsKeyHeld(currentKey)/*keyboardHookHandler->WasKeyHeld()*/)
            {
                *fileOUT << "DOWN " << currentKey << std::endl;
                std::cout << "DOWN " << currentKey << std::endl;
            }else
            {
                *fileOUT << "UP " << currentKey << std::endl;
                 std::cout << "UP " << currentKey << std::endl;
            }


            stopwatch.Start();
        }

    }
}

    KeyboardAction::~KeyboardAction()
    {
        delete fileIN;
        delete fileOUT;
    }


