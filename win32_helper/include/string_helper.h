#pragma once

#include "pch.h"
#include <memory>
#include <string>


namespace win32 {
using tstring = std::basic_string<TCHAR>;

std::unique_ptr<char[]> TCharToCChar(tstring &tstring);
}
