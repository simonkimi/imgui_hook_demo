#include "process_helper.h"
#include <psapi.h>

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
        std::cerr << std::format("OpenProcess failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    if (FindRemoteModuleHandle(hProcess.Get(), dll_path) != nullptr) {
        std::cerr << "当前模块已经被加载" << std::endl;
        return false;
    }

    SIZE_T dll_size = (lstrlen(dll_path) + 1) * sizeof(TCHAR);

    // 在注入进程中申请内存
    VirtualAllocRaii dll_addr(hProcess.Get(), dll_size);
    if (dll_addr.IsInvalid()) {
        std::cerr << std::format("VirtualAllocEx failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    // 注入dll文件名称
    if (!::WriteProcessMemory(hProcess.Get(), dll_addr.Get(), dll_path, dll_size, nullptr)) {
        std::cerr << std::format("WriteProcessMemory failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    // 获取LoadLibraryA函数地址
#ifdef UNICODE
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryW");
#else
    FARPROC load_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "LoadLibraryA");
#endif
    if (load_lib_addr == nullptr) {
        std::cerr << std::format("GetProcAddress failed, error code: {}", ::GetLastError()) << std::endl;
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
        std::cerr << std::format("CreateRemoteThread failed, error code: {}", ::GetLastError()) << std::endl;
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


std::list<std::pair<HMODULE, win32::tstring>> win32::GetProcessModules(DWORD pid)
{
    std::list<std::pair<HMODULE, win32::tstring>> module_list;
    HandleRaii handle(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));
    if (handle.IsInvalid())
        return module_list;

    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(handle.Get(), hMods, sizeof(hMods), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(handle.Get(), hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                module_list.emplace_back(hMods[i], szModName);
            }
        }
    }

    return module_list;
}

HMODULE win32::FindRemoteModuleHandle(HANDLE handle, LPCTSTR modulePath)
{
    HMODULE hMods[1024];
    DWORD cbNeeded;
    if (EnumProcessModules(handle, hMods, sizeof(hMods), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                if (lstrcmpi(szModName, modulePath) == 0) {
                    return hMods[i];
                }
            }
        }
    }
    return nullptr;
}

bool win32::CrtFreeDll(DWORD pid, LPCTSTR dll_path)
{
    HandleRaii hProcess(::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid));
    if (hProcess.Get() == nullptr) {
        std::cerr << std::format("OpenProcess failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    HMODULE dll_handle = FindRemoteModuleHandle(hProcess.Get(), dll_path);
    if (dll_handle == nullptr) {
        std::cerr << std::format("FindRemoteModuleHandle failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    FARPROC free_lib_addr = ::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")), "FreeLibrary");
    HandleRaii thread_handle(::CreateRemoteThread(
            hProcess.Get(),
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE) free_lib_addr,
            dll_handle,
            0,
            nullptr
    ));

    if (thread_handle.IsInvalid()) {
        std::cerr << std::format("CreateRemoteThread failed, error code: {}", ::GetLastError()) << std::endl;
        return false;
    }

    WaitForSingleObject(thread_handle.Get(), INFINITE);
    return true;
}


