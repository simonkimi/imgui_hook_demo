project("proj_runner")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

add_executable(proj_runner WIN32 ${SOURCE_FILES})

target_link_libraries(proj_runner PRIVATE win32_helper)