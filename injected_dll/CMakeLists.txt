project("injected_dll")

find_package(imgui CONFIG REQUIRED)

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)
include_directories(${CMAKE_SOURCE_DIR}/imgui_wrapper/include)


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
add_link_options(${PROJECT_NAME} PRIVATE -mwindows)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui imgui_wrapper win32_helper)