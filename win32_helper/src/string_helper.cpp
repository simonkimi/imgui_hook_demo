#include "string_helper.h"


std::unique_ptr<char[]> win32::TCharToCChar(tstring &tstring) {
    if (tstring.empty()) {
        return nullptr;
    }
#ifdef UNICODE
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, tstring.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::unique_ptr<char[]> charStr(new char[sizeNeeded]);
    WideCharToMultiByte(CP_UTF8, 0, tstring.c_str(), -1, charStr.get(), sizeNeeded, nullptr, nullptr);
#else
    std::unique_ptr<char[]> charStr(new char[tstring.size() + 1]);
    strcpy_s(charStr.get(), tstring.size() + 1, tstring.c_str());
#endif
    return charStr;
}