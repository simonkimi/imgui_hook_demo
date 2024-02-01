#pragma once

#include "pch.h"

#ifndef HR
#define HR(x) { HRESULT hr = (x); if (FAILED(hr)) { DxTrace(hr); } }
#endif

void DxTrace(HRESULT hresult);
