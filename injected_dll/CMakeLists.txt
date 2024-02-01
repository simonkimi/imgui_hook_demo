project("injected_dll")

find_package(imgui CONFIG REQUIRED)
find_path(DETOURS_INCLUDE_DIRS "detours/detours.h")
find_library(DETOURS_LIBRARY detours REQUIRED)


file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.h" "src/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})


target_include_directories(${PROJECT_NAME} PRIVATE ${DETOURS_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE
        win32_helper
        ${DETOURS_LIBRARY}
)
target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")