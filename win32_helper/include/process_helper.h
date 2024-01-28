#pragma once

#include "pch.h"
#include <TlHelp32.h>
#include <memory>
#include <list>
#include "string_helper.h"

namespace win32 {

DWORD FindProcessById(LPCTSTR process_name);

bool CrtInjectDll(DWORD pid, LPCTSTR dll_path);

bool CrtFreeDll(DWORD pid, LPCTSTR dll_path);

std::list<std::pair<DWORD, win32::tstring>> GetProcessList();


bool GetRemoteModuleHandle(DWORD pid, LPCTSTR modulePath, HMODULE &hModule);
}


