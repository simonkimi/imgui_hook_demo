project("qt_windows")

if (NOT DEFINED ENV{QT_HOME})
    message(FATAL_ERROR "not defined environment variable:QT_HOME")
endif ()

set(CMAKE_PREFIX_PATH $ENV{QT_HOME}/lib/cmake)
find_package(Qt6 COMPONENTS Widgets Core Gui REQUIRED)
qt_standard_project_setup()


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

qt_add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_link_options(${PROJECT_NAME} PRIVATE -mwindows)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui)

set_target_properties(helloworld PROPERTIES
        WIN32_EXECUTABLE ON
)