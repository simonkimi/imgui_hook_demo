#pragma once

#include "stdafx.h"
#include <memory>


namespace Win32Helper {
    bool OpenFileDialog(HWND hwnd, OUT LPTSTR filePath, DWORD size);
}
