#include "process_helper.h"

import raii;

DWORD win32::FindProcessById(LPCTSTR process_name)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    HandleRaii hSnapshot(::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

    if (hSnapshot.IsInvalid()) {
        return -1;
    }

    if (::Process32First(hSnapshot.Get(), &pe32)) {
        do {
            if (lstrcmpi(pe32.szExeFile, process_name) == 0) {
                return pe32.th32ProcessID;
            }
        } while (::Process32Next(hSnapshot.Get(), &pe32));
    }
    return -1;
}

bool win32::CrtInjectDll(DWORD pid, LPCTSTR dll_path)
{
    // 打开注入进程

    HandleRaii hProcess(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));

    if (hProcess.Get() == nullptr) {
        return false;
    }

    SIZE_T dll_size = (lstrlen(dll_path) + 1) * sizeof(TCHAR);

    // 在注入进程中申请内存
    VirtualAllocRaii dll_addr(hProcess.Get(), dll_size);
    if (dll_addr.IsInvalid()) {
        return false;
    }

    // 注入dll文件名称
    if (!::WriteProcessMemory(hProcess.Get(), dll_addr.Get(), dll_path, dll_size, nullptr)) {
        return false;
    }

    // 获取LoadLibraryA函数地址
#ifdef UNICODE
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryW");
#else
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
#endif
    if (load_lib_addr == nullptr) {
        return false;
    }

    // 在注入进程中创建远程线程
    HandleRaii thread_handle(::CreateRemoteThread(
            hProcess.Get(),
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE) load_lib_addr,
            dll_addr.Get(),
            0,
            nullptr
    ));
    if (thread_handle.IsInvalid()) {
        return false;
    }

    WaitForSingleObject(thread_handle.Get(), INFINITE);

    return true;
}

std::list<std::pair<DWORD, win32::tstring>> win32::GetProcessList()
{
    PROCESSENTRY32 pe32;
    std::list<std::pair<DWORD, tstring>> process_list;

    pe32.dwSize = sizeof(PROCESSENTRY32);
    HandleRaii hSnapshot(::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));

    if (hSnapshot.IsInvalid()) {
        return process_list;
    }

    if (::Process32First(hSnapshot.Get(), &pe32)) {
        do {
            process_list.emplace_back(pe32.th32ProcessID, pe32.szExeFile);
        } while (::Process32Next(hSnapshot.Get(), &pe32));
    }

    return process_list;
}

bool win32::CrtFreeDll(DWORD pid, LPCTSTR dll_path)
{
    // 打开注入进程
    HandleRaii hProcess(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));

    if (hProcess.Get() == nullptr) {
        return false;
    }

    SIZE_T dll_size = (lstrlen(dll_path) + 1) * sizeof(TCHAR);

    VirtualAllocRaii dll_addr(hProcess.Get(), dll_size);
    if (dll_addr.IsInvalid())
        return false;


    // 注入dll文件名称
    if (!::WriteProcessMemory(hProcess.Get(), dll_addr.Get(), dll_path, dll_size, nullptr)) {
        return false;
    }

    // 获取LoadLibraryA函数地址
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "FreeLibrary");
    if (load_lib_addr == nullptr) {
        return false;
    }

    // 在注入进程中创建远程线程
    HandleRaii thread_handle(::CreateRemoteThread(
            hProcess.Get(),
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE) load_lib_addr,
            dll_addr.Get(),
            0,
            nullptr
    ));
    if (thread_handle.IsInvalid()) {
        return false;
    }

    WaitForSingleObject(thread_handle.Get(), INFINITE);

    return true;
}

bool win32::GetRemoteModuleHandle(DWORD pid, LPCTSTR modulePath, HMODULE &hModule)
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
