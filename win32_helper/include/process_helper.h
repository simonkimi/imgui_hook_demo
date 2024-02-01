#pragma once

#include "pch.h"
#include <TlHelp32.h>
#include <memory>
#include <list>
#include "string_helper.h"

namespace win32 {

DWORD FindProcessById(LPCTSTR process_name);

std::list<std::pair<DWORD, win32::tstring>> GetProcessList();

std::list<std::pair<HMODULE, win32::tstring>> GetProcessModules(DWORD pid);

HMODULE FindRemoteModuleHandle(HANDLE handle, LPCTSTR modulePath);

bool CrtInjectDll(DWORD pid, LPCTSTR dll_path);

bool CrtFreeDll(DWORD pid, LPCTSTR dll_path);

HWND GetProcessWindow();

std::pair<long, long> GetWindowSize(HWND hwnd);
}


