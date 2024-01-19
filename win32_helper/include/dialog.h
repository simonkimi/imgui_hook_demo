#pragma once

#include "stdafx.h"
#include <optional>
#include <string>
#include <memory>


std::unique_ptr<std::string> OpenFileDialog(HWND__ *hwnd);