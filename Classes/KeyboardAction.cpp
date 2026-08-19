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

void KeyboardAction::HoldKey(WORD keyCode)
{
    // Prevent starting multiple hold threads for the same key
    if (keyHeld.exchange(true))
        return;

    keyThread = std::thread([this, keyCode]()
    {
        INPUT input{};
        input.type = INPUT_KEYBOARD;
        input.ki.wVk = keyCode;
        input.ki.dwFlags = 0;

        // Initial key-down
        SendInput(1, &input, sizeof(INPUT));

        // Approximate keyboard auto-repeat
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        while (keyHeld)
        {
            SendInput(1, &input, sizeof(INPUT));

            std::this_thread::sleep_for(
                std::chrono::milliseconds(30)
            );
        }
    });
}

void KeyboardAction::ReleaseKey(WORD keyCode)
{
    keyHeld = false;

    if (keyThread.joinable())
        keyThread.join();

    INPUT input{};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = keyCode;
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}







void KeyboardAction::RecordAction(KeyboardHookHandler *keyboardHookHandler, std::atomic<bool>* shoulldTerminate)
{

    if (!fileOUT || !keyboardHookHandler) return;

    stopwatch.Start();

    while (!shoulldTerminate->load())
    {
        currentKey = keyboardHookHandler->GetLastKey();

        if (currentKey)
        {
            time = stopwatch.GetTime();
            stopwatch.Stop();
            *fileOUT << time << std::endl;

            if (keyboardHookHandler->IsKeyHeld(currentKey)/*keyboardHookHandler->WasKeyHeld()*/)
            {
                *fileOUT << "DOWN " << currentKey << std::endl;
            }else
            {
                *fileOUT << "UP " << currentKey << std::endl;
            }


            stopwatch.Start();
        }
    }
}




void KeyboardAction::RecordSequence(KeyboardHookHandler *keyboardHookHandler, std::atomic<bool> *shoulldTerminate)
{
    if (!fileOUT || !keyboardHookHandler) return;

    while (!shoulldTerminate->load())
    {
        currentKey = keyboardHookHandler->GetLastKey();

        if (keyboardHookHandler->IsKeyHeld(currentKey))
            *fileOUT << currentKey << std::endl;
    }
}

void KeyboardAction::CreateSequence()
{
    if (!fileIN)
        return;

    std::thread([this]()
    {
        std::lock_guard lock(keyQueueMutex);

        fileIN->clear();
        fileIN->seekg(0);

        keyQueue = std::queue<WORD>();

        WORD key;

        while (*fileIN >> key)
        {
            keyQueue.push(key);
        }

        sequenceLength = keyQueue.size();

    }).detach();
}
void KeyboardAction::CheckSequence(
    KeyboardHookHandler* keyboardHookHandler,
    double maxTime)
{
    if (!keyboardHookHandler)
        return;

    std::lock_guard lock(keyQueueMutex);

    if (keyQueue.empty())
        return;

    currentKey = keyboardHookHandler->GetLastKey();

    if (currentKey == keyQueue.front())
    {
        keyQueue.pop();

        if (keyQueue.empty())
            InitiateSequenceAction();
    }
    else if (sequenceLength != keyQueue.size())
    {
        if (stopwatch.IsRunning())
        {
            if ((maxTime - stopwatch.GetTime()) < 0)
            {
                stopwatch.Stop();
                CreateSequence();
            }
        }
        else
        {
            stopwatch.Start();
        }
    }
}

void KeyboardAction::DoAction()
{
    if (!fileIN) return;

    // std::this_thread::sleep_for(std::chrono::seconds(3));
    //
    // HoldKey(0x44);
    //
    // std::cout << "started holding D\n";
    //
    //
    //     std::this_thread::sleep_for(std::chrono::seconds(7));
    //
    //
    // ReleaseKey(0x44);
    //
    // std::cout << "stopped holding D\n";


    while(!fileIN->eof())
    {
        *fileIN >> time;

        *fileIN >> currentCommand >> currentKey;

       // std::cout << time << "\t" << currentKey << "\t" << currentCommand << std::endl;

        std::chrono::duration<double> duration(time);
        std::this_thread::sleep_for(duration);

        if (currentCommand.find("DOWN") != std::string::npos)
        {
            std::cout << currentKey << " Is down"<< std::endl;
            HoldKey(currentKey);
        } else
        {
            ReleaseKey(currentKey);
            std::cout << currentKey << " Is up"<< std::endl;
        }

    }
}

KeyboardAction::~KeyboardAction()
{
    delete fileIN;
    delete fileOUT;
}


