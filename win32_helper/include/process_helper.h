#pragma once

#include "stdafx.h"
#include <TlHelp32.h>
#include <memory>
#include <list>
#include "string_helper.h"

namespace Win32Helper {
    DWORD FindProcessById(LPCTSTR process_name);

    bool CreateRemoteThreadInjectDll(DWORD pid, LPCTSTR dll_path);

    std::list<std::tuple<DWORD, tstring>> GetProcessList();
}


