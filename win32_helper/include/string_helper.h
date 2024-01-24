#pragma once

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <string>


namespace win32 {
using tstring = std::basic_string<TCHAR>;

std::unique_ptr<char[]> TCharToCChar(tstring &tstring);
}
