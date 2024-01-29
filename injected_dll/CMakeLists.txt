project("injected_dll")

find_package(imgui CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE win32_helper)
target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")