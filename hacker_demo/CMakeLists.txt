find_package(imgui CONFIG REQUIRED)

project("windows_demo")

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/imgui_impl/*.h" "lib/imgui_impl/*.cpp")


add_executable(${CMAKE_PROJECT_NAME} ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})


target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui d3d11 d3dcompiler dxguid dwmapi)