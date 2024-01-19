#include "../include/string_helper.h"


void TCharToCChar(IN TCHAR *tchar, int size, OUT char *cchar) {
#ifdef UNICODE
    WideCharToMultiByte(CP_UTF8, 0, tchar, -1, cchar, size, nullptr, nullptr);
#else
    strncpy(cchar, tchar, size);
#endif
}