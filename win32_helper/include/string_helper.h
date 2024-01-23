#pragma once

#include "stdafx.h"
#include <memory>
#include <string>


using tstring = std::basic_string<TCHAR>;

void TCharToCChar(IN LPTSTR tchar, int size, OUT char *cchar);

#ifdef UNICODE
#define tcout std::wcout
#else
#define tcout std::cout
#endif
