project("d3d_demo")

include_directories("include")

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.h" "src/*.cpp")
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
add_link_options(${PROJECT_NAME} PRIVATE -mwindows)

target_link_libraries(${PROJECT_NAME} PRIVATE
        d3d11
        d3dcompiler
        dxguid dwmapi
        win32_helper
)

target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")