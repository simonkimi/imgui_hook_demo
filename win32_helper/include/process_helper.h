#pragma once

#include "stdafx.h"
#include <TlHelp32.h>
#include <memory>
#include <list>
#include "string_helper.h"

namespace win32 {

DWORD FindProcessById(LPCTSTR process_name);

bool CrtInjectDll(DWORD pid, LPCTSTR dll_path);

bool CrtFreeDll(DWORD pid, LPCTSTR dll_path);

std::list<std::pair<DWORD, win32::tstring>> GetProcessList();


bool GetRemoteModuleHandle(DWORD pid, LPCTSTR modulePath, HMODULE &hModule)
{
    hModule = nullptr;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 me32;
        me32.dwSize = sizeof(MODULEENTRY32);
        if (Module32First(hSnapshot, &me32)) {
            do {
                if (_tcsicmp(me32.szExePath, modulePath) == 0) {
                    hModule = me32.hModule;
                    break;
                }
            } while (Module32Next(hSnapshot, &me32));
        }
        CloseHandle(hSnapshot);
    }
    return (hModule != nullptr);
}
}


