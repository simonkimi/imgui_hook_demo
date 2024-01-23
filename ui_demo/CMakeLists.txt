find_package(imgui CONFIG REQUIRED)

project("ui_demo")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "include/*.h")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/imgui_impl/*.h" "lib/imgui_impl/*.cpp")

add_executable(ui_demo WIN32 ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})
add_link_options(ui_demo PRIVATE -mwindows)

target_link_libraries(ui_demo PRIVATE
        win32_helper
        imgui::imgui 
        d3d11 
        d3dcompiler 
        dxguid dwmapi
)