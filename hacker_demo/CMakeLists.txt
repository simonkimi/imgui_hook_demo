find_package(imgui CONFIG REQUIRED)

project("windows_demo")

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/imgui_impl/*.h" "lib/imgui_impl/*.cpp")

file(GLOB_RECURSE MODULES_FILES "src/*.cppm")


add_executable(${CMAKE_PROJECT_NAME} WIN32 ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})
add_link_options(${CMAKE_PROJECT_NAME} PRIVATE -mwindows)

target_sources(${CMAKE_PROJECT_NAME} PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${MODULES_FILES})

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui d3d11 d3dcompiler dxguid dwmapi)