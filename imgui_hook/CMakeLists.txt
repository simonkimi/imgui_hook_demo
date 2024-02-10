project("imgui_hook")

find_package(imgui CONFIG REQUIRED)
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

include_directories("include")
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
include_directories("imgui")

file(GLOB_RECURSE LIB_FILES "lib/*.cpp" "lib/*.h")
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_library(${PROJECT_NAME} SHARED ${LIB_FILES} ${SOURCE_FILES} ${IMGUI_WIN32_IMPL})


set(RENDER_API "OpenGL" CACHE STRING "Select the rendering API")
set_property(CACHE RENDER_API PROPERTY STRINGS "OpenGL" "DX11")


file(GLOB_RECURSE IMGUI_WIN32_IMPL "imgui/backends/*win32*.cpp" , "imgui/backends/*win32*.h")
file(GLOB IMGUI_RENDER_CORE "imgui/*.cpp" "imgui/*.h")
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_RENDER_CORE} ${IMGUI_WIN32_IMPL})
if (RENDER_API STREQUAL "OpenGL")
    file(GLOB_RECURSE OPENGL_FILES "imgui/backends/*opengl*.cpp" "imgui/backends/*opengl*.h")
    file(GLOB_RECURSE OPENGL_IMPL_FILES "imgui/impl/*opengl*.cpp" "imgui/impl/*opengl*.h")
    target_sources(${PROJECT_NAME} PRIVATE ${OPENGL_FILES} ${OPENGL_IMPL_FILES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDER_API_OPENGL)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
elseif (RENDER_API STREQUAL "DX11")
    file(GLOB_RECURSE DX11_FILES "imgui/backends/*dx11*.cpp" "imgui/backends/*dx11*.h")
    file(GLOB_RECURSE DX11_IMPL_FILES "imgui/impl/*dx11*.cpp" "imgui/impl/*dx11*.h")
    target_sources(${PROJECT_NAME} PRIVATE ${DX11_FILES} ${DX11_IMPL_FILES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE RENDER_API_DX11)
    target_link_libraries(${PROJECT_NAME} PRIVATE d3d11 d3dcompiler dxguid dwmapi)
endif ()

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")
target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE
        imgui::imgui
        win32_helper
        ${DETOURS_LIBRARY}
        fmt::fmt-header-only
        spdlog::spdlog_header_only
)

