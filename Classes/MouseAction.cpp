//
// Created by Mihnwq on 8/9/2026.
//

#include "../Headers/MouseAction.h"

#include <iostream>
#include <ostream>
#include <windows.h>
#include <thread>
#include <chrono>
using namespace std;


void MouseAction::SetFile(string fileName)
{
    fileOUT = new fstream(fileName.c_str(), ios::in | ios::out | ios::trunc);
}

void MouseAction::ReadFile(string fileName)
{
    fileIN = new fstream(fileName.c_str(), ios::in | ios::out);
    fileIN->seekg(0);
}

void MouseAction::MouseButton(DWORD button) const
{
    INPUT input = {};

    input.type = INPUT_MOUSE;
    input.mi.dwFlags = button;

    SendInput(1, &input, sizeof(INPUT));
}

void MouseAction::LeftClick() const
{
    MouseButton(MOUSEEVENTF_LEFTDOWN);
    MouseButton(MOUSEEVENTF_LEFTUP);
}

void MouseAction::RightClick() const
{
    MouseButton(MOUSEEVENTF_RIGHTDOWN);
    MouseButton(MOUSEEVENTF_RIGHTUP);
}

void MouseAction::Scroll(string direction) const
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = (direction.find("UP") != string::npos)? 120 : -120;

    SendInput(1, &input, sizeof(INPUT));

}



void MouseAction::RecordAction(MouseHookHandler* hook,std::atomic<bool>* shouldTerminate)
{

    if (!fileOUT || !hook) return;

    while (!shouldTerminate->load())
    {

        GetCursorPos(&pos);

        long x = pos.x;
        long y = pos.y;

        bool leftPressed = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        bool rightPressed = GetAsyncKeyState(VK_RBUTTON) & 0x8000;

        *fileOUT <<"MOVE "<< x << " " << y << "\n";

        if (leftPressed && !previousLeftPressed)
        {
            *fileOUT << "Left_Click" <<"\n";

        }
        else if (rightPressed && !previousRightPressed)
        {
            *fileOUT << "Right_Click" <<"\n";

        }

        int scrollAmount = hook->GetScrollAmount(); //GetScrollAmount();

        if (scrollAmount > 0)
        {
            *fileOUT << "Scroll_Up " << "\n";


        }else if (scrollAmount < 0)
        {
            *fileOUT << "Scroll_Down " << "\n";

        }

        previousLeftPressed = leftPressed;
        previousRightPressed = rightPressed;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void MouseAction::DoAction(int speed)
{
    if (!fileIN) return;

    string action;

    speed = (speed < 1000) ? speed : 1000;

     while (*fileIN >> action)
     {
         if (action.find("MOVE") != string::npos)
         {
             *fileIN >> pos.x;
             *fileIN >> pos.y;

             SetCursorPos(pos.x, pos.y);
         }
         else if (action.find("Left_Click") != string::npos)
         {
             LeftClick();
         }else if (action.find("Right_Click") != string::npos)
         {
             RightClick();
         }
         else if (action.find("Scroll_Up") != string::npos)
         {
             Scroll("UP");
         }else if (action.find("Scroll_Down") != string::npos)
         {
             Scroll("DOWN");
         }

         this_thread::sleep_for(std::chrono::microseconds(speed));
     }


}


MouseAction::~MouseAction()
{


    delete fileOUT;
    delete fileIN;

  //  cout << "Deletion successful." << endl;
}
