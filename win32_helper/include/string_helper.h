#pragma once

#include "stdafx.h"
#include <memory>
#include <string>


typedef std::basic_string<TCHAR> tstring;

void TCharToCChar(IN LPTSTR tchar, int size, OUT char *cchar);