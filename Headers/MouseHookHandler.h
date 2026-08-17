//
// Created by Mihnwq on 8/13/2026.
//

#pragma once
#include <windows.h>
#include <fstream>
#include <iostream>
#include <thread>

class MouseHookHandler
{

    HHOOK mouseHook = nullptr;
    std::thread scrollThread;
    DWORD scrollThreadID = 0;

    std::atomic<int> scrollAmount = 0;

    inline static MouseHookHandler* instance = nullptr;

    public:
    MouseHookHandler();

    static LRESULT CALLBACK MouseHookCallback(
         int nCode,
         WPARAM wParam,
         LPARAM lParam
     );

    int GetScrollAmount() const;

    ~MouseHookHandler();
};
