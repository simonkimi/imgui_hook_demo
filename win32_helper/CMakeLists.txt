project("win32_helper")

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.cpp")

include_directories(include)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

