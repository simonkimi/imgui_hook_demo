#pragma once

#include "stdafx.h"
#include <memory>
#include <optional>
#include "string_helper.h"


namespace win32 {

bool OpenFileDialog(HWND hwnd, OUT LPTSTR filePath, DWORD size);

std::optional<win32::tstring> OpenFileDialog(HWND hwnd, LPCTSTR filter);

}
