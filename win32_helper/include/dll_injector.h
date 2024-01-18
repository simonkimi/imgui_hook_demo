#pragma once


#include "stdafx.h"
#include <TlHelp32.h>


DWORD FindProcessById(LPCTSTR process_name);

bool CreateRemoteThreadInjectDll(DWORD pid, LPCTSTR dll_path);

