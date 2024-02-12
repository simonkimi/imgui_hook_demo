#pragma once

#include "pch.h"
#include <string>


#ifndef HR
#define HR(x) { HRESULT hr = (x); if (FAILED(hr)) { DxTrace(hr); } }
#endif

#ifndef RELEASE_COM
#define RELEASE_COM(x) { if (x) { x->Release(); x = nullptr; } }
#endif

std::unique_ptr<TCHAR[]> GetHRResult(HRESULT hresult);

void DxTrace(HRESULT hresult, bool use_msgbox = false);