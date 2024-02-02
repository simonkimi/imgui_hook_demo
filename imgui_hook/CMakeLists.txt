project("imgui_hook")

find_package(imgui CONFIG REQUIRED)
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/*.cpp")

include_directories("include")
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)

add_library(${PROJECT_NAME} SHARED ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui::imgui
        d3d11
        d3dcompiler
        dxguid dwmapi
        win32_helper
        ${DETOURS_LIBRARY}
)

