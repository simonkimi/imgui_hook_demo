find_package(imgui CONFIG REQUIRED)

project("win32_helper")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_link_directories(${PROJECT_NAME} PUBLIC HEADER_FILES)