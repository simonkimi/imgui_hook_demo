#pragma once

#include "stdafx.h"
#include <memory>


namespace win32 {
    bool OpenFileDialog(HWND hwnd, OUT LPTSTR filePath, DWORD size);
}
