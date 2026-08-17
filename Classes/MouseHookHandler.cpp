//
// Created by Mihnwq on 8/13/2026.
//
#include "../Headers/MouseHookHandler.h"

#include <iostream>



MouseHookHandler::MouseHookHandler()
{
    instance = this;

    scrollThread = std::thread([this]()
   {
       scrollThreadID = GetCurrentThreadId();

       mouseHook = SetWindowsHookEx(
           WH_MOUSE_LL,
           MouseHookCallback,
           GetModuleHandle(nullptr),
           0
       );

       MSG msg;

       while (GetMessage(&msg, nullptr, 0, 0))
       {
           TranslateMessage(&msg);
           DispatchMessage(&msg);
       }
   });
}

LRESULT MouseHookHandler::MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_MOUSEWHEEL)
    {
        MSLLHOOKSTRUCT* mouse = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        instance->scrollAmount += GET_WHEEL_DELTA_WPARAM(mouse->mouseData);
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int MouseHookHandler::GetScrollAmount() const
{
  return instance->scrollAmount.exchange(0);
}

MouseHookHandler::~MouseHookHandler()
{
    if (scrollThreadID != 0)
    {
        PostThreadMessage(scrollThreadID, WM_QUIT, 0, 0);
    }

    if (scrollThread.joinable())
    {
        scrollThread.join();
    }

    if (mouseHook != nullptr)
    {
        UnhookWindowsHookEx(mouseHook);
        mouseHook = nullptr;
    }
}