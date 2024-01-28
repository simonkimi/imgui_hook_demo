project("win32_helper")

file(GLOB_RECURSE SOURCE_FILES "include/*.h" "src/*.cpp")
file(GLOB_RECURSE MODULES_FILES "module/*.ixx")

include_directories(include)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_sources(${PROJECT_NAME} PUBLIC FILE_SET modules TYPE CXX_MODULES FILES ${MODULES_FILES})
target_precompile_headers(${PROJECT_NAME} PRIVATE "include/pch.h")