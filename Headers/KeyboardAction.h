#pragma once

#include <fstream>

#include "KeyboardHookHandler.h"
#include <fstream>
#include <queue>

#include "Stopwatch.h"


class KeyboardAction
{
    std::fstream* fileIN;
    std::fstream* fileOUT;

    WORD currentKey;
    double time;
    std::string currentCommand;

    std::atomic<bool> keyHeld{false};
    std::thread keyThread;

    std::queue<WORD> keyQueue;
    std::mutex keyQueueMutex;
    int sequenceLength;

    Stopwatch stopwatch;

    public:
        KeyboardAction() = default;

        void PressKey(WORD keyCode) const;
        void HoldKey(WORD keyCode);
        void ReleaseKey(WORD keyCode);

        void SetFile(std::string filename);
        void ReadFile(std::string filename);


        void RecordAction(KeyboardHookHandler* keyboardHookHandler, std::atomic<bool>* shoulldTerminate);
        void RecordSequence(KeyboardHookHandler* keyboardHookHandler, std::atomic<bool>* shoulldTerminate);

        void CreateSequence();
        void CheckSequence(KeyboardHookHandler* keyboardHookHandler , double maxTime = 1000);
        void InitiateSequenceAction();

        void DoAction();


        ~KeyboardAction();
};
