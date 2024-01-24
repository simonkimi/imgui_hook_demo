project("proj_runner")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")


include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE win32_helper)