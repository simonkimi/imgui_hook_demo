find_package(imgui CONFIG REQUIRED)

project("win32_helper")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

include_directories(include)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

set_target_properties(win32_helper PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS TRUE
)

set_target_properties(win32_helper PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")