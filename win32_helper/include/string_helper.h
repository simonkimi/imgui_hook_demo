#pragma once

#include "stdafx.h"
#include <iostream>
#include <memory>
#include <string>


namespace win32 {
    using tstring = std::basic_string<TCHAR>;

    void TCharToCChar(IN LPTSTR tchar, int size, OUT char *cchar);
}
