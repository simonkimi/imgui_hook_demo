project("console_app")


file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")

include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE d3d9 d3dcompiler dxguid dwmapi)