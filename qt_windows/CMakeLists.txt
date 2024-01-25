project("qt_windows")

if (NOT DEFINED ENV{QT_HOME})
    message(FATAL_ERROR "not defined environment variable:QT_HOME")
endif ()

find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/lib/cmake)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
qt_standard_project_setup()

include_directories(include)
include_directories(${CMAKE_BINARY_DIR}/${PROJECT_NAME})

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE UI_FILES "src/*.ui")

qt6_wrap_ui(WRAP_FILES ${UI_FILES})

SOURCE_GROUP("ui" FILES ${UI_FILES} ${WRAP_FILES})
qt_add_executable(${PROJECT_NAME} ${UI_FILES} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)
set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
)