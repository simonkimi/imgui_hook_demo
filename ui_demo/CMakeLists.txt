find_package(imgui CONFIG REQUIRED)

project("ui_demo")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE IMGUI_SOURCE_FILES "lib/*.h" "lib/*.cpp")

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

set(DLL_PATH "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "DLLs")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_PATH}/win32_helper.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)