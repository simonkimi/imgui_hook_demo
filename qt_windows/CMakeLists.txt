project("qt_windows")

if (NOT DEFINED ENV{QT_HOME})
    message(FATAL_ERROR "not defined environment variable:QT_HOME")
endif ()

find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)

set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/lib/cmake)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

include_directories(include)
include_directories(${CMAKE_SOURCE_DIR}/win32_helper/include)

file(GLOB_RECURSE SOURCE_FILES "src/*.h" "src/*.cpp")
file(GLOB_RECURSE UI_FILES "ui/*.ui")

qt_add_executable(${PROJECT_NAME} ${UI_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
target_link_libraries(${PROJECT_NAME} PRIVATE win32_helper)

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
)

target_precompile_headers(${PROJECT_NAME} PRIVATE include/pch.h)
