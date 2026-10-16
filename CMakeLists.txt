cmake_minimum_required(VERSION 3.16)
project(NEXYTerminal VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/TerminalWidget.cpp
    src/TerminalSession.cpp
    src/SettingsDialog.cpp
    src/Settings.cpp
)

set(HEADERS
    src/MainWindow.h
    src/TerminalWidget.h
    src/TerminalSession.h
    src/SettingsDialog.h
    src/Settings.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(NEXYTerminal WIN32
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Link Qt libraries
target_link_libraries(NEXYTerminal PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
)

# Set output directory
set_target_properties(NEXYTerminal PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Windows-specific settings
if(WIN32)
    target_compile_definitions(NEXYTerminal PRIVATE UNICODE _UNICODE)
endif()
