find_package(imgui CONFIG REQUIRED)

project("ui_demo")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/imgui_impl/*.h" "lib/imgui_impl/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)


add_executable(${PROJECT_NAME} WIN32 ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})
add_link_options(${PROJECT_NAME} PRIVATE -mwindows)

target_link_libraries(${PROJECT_NAME} PRIVATE
        win32_helper
        imgui::imgui 
        d3d11 
        d3dcompiler 
        dxguid dwmapi
)