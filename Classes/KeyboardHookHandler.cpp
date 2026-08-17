//
// Created by Mihnwq on 8/16/2026.
//

#include "../Headers/KeyboardHookHandler.h"

void KeyboardHookHandler::Run()
{
    threadId = GetCurrentThreadId();

    hook = SetWindowsHookEx(
        WH_KEYBOARD_LL,
        HookProc,
        GetModuleHandle(nullptr),
        0
    );

    MSG msg;
    while (running && GetMessage(&msg, nullptr, 0, 0))
    {
    }

    UnhookWindowsHookEx(hook);
}

KeyboardHookHandler::KeyboardHookHandler()
{
    instance = this;

    thread = std::thread(&KeyboardHookHandler::Run, this);
}

LRESULT CALLBACK KeyboardHookHandler::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        auto* key = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        DWORD vk = key->vkCode;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            if (instance->keysDown[vk] == 0)
            {
                instance->keysDown[vk] = 1;
                instance->lastKeyDown = true;
                instance->lastKey = key->vkCode;
                instance->keysPressed[vk] = true;
            }
            else instance->lastKey = 0;
        }else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            instance->keysPressed[vk] = false;
            instance->keysDown[vk] = 0;
            instance->lastKey = key->vkCode;
            instance->lastKeyDown = false;
        }
    }



    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

KeyboardHookHandler::~KeyboardHookHandler()
{
    running = false;

    while (threadId == 0)
        std::this_thread::yield();

    PostThreadMessage(threadId, WM_QUIT, 0, 0);

    if (thread.joinable())
        thread.join();
}

int KeyboardHookHandler::GetLastKey() const
{
    int lastKey = instance->lastKey.exchange(0);

    if (instance->keysDown[lastKey] < 2)
    {
        if (instance->keysDown[lastKey] == 1)
            instance->keysDown[lastKey] = 2;

        return lastKey;
    }

    return 0;
}

bool KeyboardHookHandler::IsKeyHeld(int vkCode) const
{
    return instance->keysPressed[vkCode].load();
}

bool KeyboardHookHandler::WasKeyHeld() const {
    return instance->lastKeyDown.load();
}
