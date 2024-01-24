project("proj_runner")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp" "src/*.h")

include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
add_executable(${PROJECT_NAME} WIN32 ${SOURCE_FILES})

set(DLL_PATH "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "DLLs")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_PATH}/win32_helper.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
target_link_libraries(${PROJECT_NAME} PRIVATE win32_helper)