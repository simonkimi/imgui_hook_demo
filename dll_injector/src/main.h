#pragma once

#define DllExport extern "C" __declspec(dllexport)

#include <windows.h>
#include <TlHelp32.h>
#include <tchar.h>

DllExport DWORD FindProcessById(LPCTSTR process_name);

DllExport bool CreateRemoteThreadInjectDll(DWORD pid, LPCTSTR dll_path);

