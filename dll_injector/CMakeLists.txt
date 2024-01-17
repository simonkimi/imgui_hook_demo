find_package(imgui CONFIG REQUIRED)

project("dll_injector")

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp" "src/*.hpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})