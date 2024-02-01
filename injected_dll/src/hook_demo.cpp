#include "hook_demo.h"

int (*OldMessageBoxW)(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) = MessageBoxW;

int HookedMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
    std::cout << "Hooked MessageBoxW" << std::endl;
    return OldMessageBoxW(hWnd, L"Hook完毕", lpCaption, uType);
}

void StartHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID &) OldMessageBoxW, HookedMessageBoxW);
    DetourTransactionCommit();
}

void EndHook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &) OldMessageBoxW, HookedMessageBoxW);
    DetourTransactionCommit();
}
