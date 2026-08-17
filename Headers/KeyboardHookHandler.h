//
// Created by Mihnwq on 8/16/2026.
//

#ifndef INPUTMONITORUI_KEYBOARDHOOKHANDLER_H
#define INPUTMONITORUI_KEYBOARDHOOKHANDLER_H
#include <array>

#endif //INPUTMONITORUI_KEYBOARDHOOKHANDLER_H
#pragma once

#include <windows.h>
#include <thread>
#include <atomic>

class KeyboardHookHandler
{
private:
    HHOOK hook = nullptr;
    std::thread thread;
    std::atomic<int> lastKey = 0;
    std::atomic<DWORD> threadId = 0;
    std::atomic<bool> running = true;
    std::array<std::atomic_bool, 256> keysPressed{};
    std::array<std::atomic_int, 256> keysDown{};
    std::atomic<bool> lastKeyDown{false};

    static inline  KeyboardHookHandler* instance = nullptr;

    static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);

    void Run();

public:
    KeyboardHookHandler();

    ~KeyboardHookHandler();

    int GetLastKey() const;

    bool IsKeyHeld(int keyCode) const;

    bool WasKeyHeld() const;
};