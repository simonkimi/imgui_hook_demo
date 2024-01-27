find_package(imgui CONFIG REQUIRED)
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)

project("imgui_hook")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/*.h" "lib/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)


add_library(${PROJECT_NAME} ${IMGUI_SOURCE_FILES} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui::imgui
        d3d11
        d3dcompiler
        dxguid dwmapi
        ${DETOURS_LIBRARY}
)