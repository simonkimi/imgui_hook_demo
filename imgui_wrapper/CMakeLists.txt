find_package(imgui CONFIG REQUIRED)

project("imgui_wrapper")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/*.h" "lib/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)

add_library(${PROJECT_NAME} ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui::imgui
        d3d11
        d3dcompiler
        dxguid dwmapi
)
