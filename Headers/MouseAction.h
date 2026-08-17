#pragma once
#include <windows.h>
#include <fstream>
#include <iostream>
#include <thread>

#include "MouseHookHandler.h"

using namespace std;

class MouseAction
{
    POINT pos;

    fstream* fileOUT = NULL;
    fstream* fileIN = NULL;

    bool previousLeftPressed = false;
    bool previousRightPressed = false;

    void MouseButton(DWORD button) const;

    public:
        MouseAction() = default;

        void SetFile(string fileName);
        void ReadFile(string fileName);
        void RecordAction(MouseHookHandler* hook , std::atomic<bool>* shouldTerminate);
        //Speed at 1k is the best default one.
        void DoAction(int speed = 1000);

        void LeftClick() const;
        void RightClick() const;
        void Scroll(string direction) const;


        ~MouseAction();

};