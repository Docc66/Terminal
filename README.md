# NEXY Terminal

A modern, minimalist terminal application for Windows built with C++ and Qt 6.

## Features

- **Clean, Professional Interface**: Dark theme with minimal design inspired by modern developer tools
- **Multiple Terminal Tabs**: Work with multiple terminal sessions simultaneously
- **Real Shell Integration**: Full integration with Windows PowerShell and Command Prompt
- **Customizable Settings**: 
  - Dark/Light themes
  - Font family and size selection
  - Background and text color customization
  - Cursor style options
  - Default shell selection
  - Scrollback buffer configuration
  - Window opacity control
- **Command History**: Navigate through previous commands using arrow keys
- **Keyboard Shortcuts**: Full support for copy/paste and standard terminal shortcuts
- **Status Bar**: Displays current working directory
- **Process Management**: Proper handling of running processes when closing tabs

## Requirements

- Windows 10 or Windows 11
- Qt 6.2 or later
- CMake 3.16 or later
- Visual Studio 2019 or later (with C++ development tools)

## Building from Source

### 1. Install Prerequisites

#### Install Qt 6
Download and install Qt 6 from [https://www.qt.io/download](https://www.qt.io/download)

Make sure to install:
- Qt 6.x for MSVC compiler
- Qt Creator (optional, but recommended)

#### Install CMake
Download from [https://cmake.org/download/](https://cmake.org/download/) or install via Visual Studio Installer.

### 2. Build the Project

#### Option A: Using Command Line

1. Open "x64 Native Tools Command Prompt for VS 2019" (or your Visual Studio version)

2. Navigate to the project directory:
```cmd
cd path\to\NEXYTerminal
```

3. Set Qt path (replace with your Qt installation path):
```cmd
set Qt6_DIR=C:\Qt\6.x.x\msvc2019_64\lib\cmake\Qt6
```

4. Create build directory and configure:
```cmd
mkdir build
cd build
cmake ..
```

5. Build the project:
```cmd
cmake --build . --config Release
```

6. The executable will be in `build\bin\Release\NEXYTerminal.exe`

#### Option B: Using Qt Creator

1. Open Qt Creator
2. File → Open File or Project
3. Select `CMakeLists.txt`
4. Configure the project with your Qt Kit
5. Build → Build Project "NEXYTerminal"

### 3. Running the Application

After building, run:
```cmd
build\bin\Release\NEXYTerminal.exe
```

Or simply double-click the executable in Windows Explorer.

**Note**: Make sure the Qt DLLs are in your PATH, or copy them to the same directory as the executable. You can also use `windeployqt` to deploy the required files:

```cmd
cd build\bin\Release
windeployqt NEXYTerminal.exe
```

## Usage

### Basic Operations

- **New Tab**: Click "+ New Tab" in toolbar or press `Ctrl+T`
- **Close Tab**: Click the X on a tab or press `Ctrl+W`
- **Switch Tabs**: Click on tabs or use `Ctrl+Tab`
- **Copy**: Select text and press `Ctrl+C`
- **Paste**: Press `Ctrl+V`
- **Command History**: Use `Up` and `Down` arrow keys

### Settings

Access settings via:
- Menu: File → Settings
- Toolbar: Click "⚙ Settings"
- Keyboard: Press `Ctrl+,`

Configure:
- Theme (Dark/Light)
- Font family and size
- Custom colors
- Cursor style
- Default shell (PowerShell or Command Prompt)
- Scrollback buffer size
- Window opacity

### Keyboard Shortcuts

- `Ctrl+T` - New Tab
- `Ctrl+W` - Close Tab
- `Ctrl+,` - Settings
- `Ctrl+C` - Copy (or send interrupt signal if no selection)
- `Ctrl+V` - Paste
- `Alt+F4` - Exit application
- `Up/Down` - Navigate command history

## Project Structure

```
NEXYTerminal/
├── src/
│   ├── main.cpp              # Application entry point
│   ├── MainWindow.h/cpp      # Main application window
│   ├── TerminalWidget.h/cpp  # Terminal display and input
│   ├── TerminalSession.h/cpp # Shell process management
│   ├── SettingsDialog.h/cpp  # Settings interface
│   └── Settings.h/cpp        # Settings storage and management
├── resources/
│   └── resources.qrc         # Qt resources file
├── CMakeLists.txt            # CMake build configuration
└── README.md                 # This file
```

## Architecture

- **MainWindow**: Manages the application window, menu bar, toolbar, and tab widget
- **TerminalWidget**: Handles terminal display, user input, and keyboard events
- **TerminalSession**: Manages the shell process (PowerShell/CMD) and I/O communication
- **Settings**: Stores and manages user preferences
- **SettingsDialog**: Provides UI for customizing application settings

## Technical Details

- **Programming Language**: C++17
- **UI Framework**: Qt 6 Widgets
- **Build System**: CMake
- **Shell Integration**: QProcess with proper I/O handling
- **Settings Storage**: QSettings (Windows Registry)

## Troubleshooting

### Build Issues

**CMake can't find Qt6**:
- Make sure Qt6_DIR is set correctly
- Verify Qt 6 is installed with MSVC compiler support

**Linker errors**:
- Ensure you're using the correct Visual Studio version matching your Qt installation
- Open the correct Native Tools Command Prompt

### Runtime Issues

**Application won't start**:
- Run `windeployqt NEXYTerminal.exe` in the executable directory
- Or add Qt bin directory to PATH

**Shell commands don't work**:
- Verify PowerShell and CMD are available in system PATH
- Check Windows permissions for process execution

## Future Enhancements

Potential features for future versions:
- Split pane support
- Custom key bindings
- Profile/session management
- Search functionality
- SSH integration
- Theme import/export

## License

This project is provided as-is for educational and development purposes.

## Contributing

This is a demonstration project. Feel free to fork and modify for your own needs.

## Author

Created as a modern terminal application for Windows developers.
