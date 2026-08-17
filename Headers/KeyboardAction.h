#pragma once

#include <fstream>

#include "KeyboardHookHandler.h"
#include <fstream>

#include "Stopwatch.h"


class KeyboardAction
{
    std::fstream* fileIN;
    std::fstream* fileOUT;

    Stopwatch stopwatch;

    public:
        KeyboardAction() = default;

        void PressKey(WORD keyCode) const;
        void HoldKey(WORD keyCode) const;
        void ReleaseKey(WORD keyCode) const;

        void SetFile(std::string filename);
        void ReadFile(std::string filename);


        void RecordAction(KeyboardHookHandler* keyboardHookHandler, std::atomic<bool>* shoulldTerminate);
        void DoAction();


        ~KeyboardAction();
};
