# NEXY Terminal - Build Instructions for Windows

This guide will help you build NEXY Terminal from source on Windows.

## Prerequisites

### 1. Install Visual Studio

Download and install **Visual Studio 2019** or **Visual Studio 2022** with the following components:
- Desktop development with C++
- MSVC v142 or v143 compiler
- Windows 10 SDK (10.0.19041.0 or later)

**Download**: https://visualstudio.microsoft.com/downloads/

### 2. Install Qt 6

1. Download the Qt Online Installer from: https://www.qt.io/download-qt-installer
2. Run the installer and sign in (or create a free Qt account)
3. Select Qt 6.5 or later for installation
4. Choose the following components:
   - **MSVC 2019 64-bit** (or MSVC 2022 64-bit if using VS 2022)
   - Qt 6.x Prebuilt Components for MSVC

Example installation path: `C:\Qt\6.5.3\msvc2019_64`

### 3. Install CMake

Download and install CMake 3.16 or later:
- **Download**: https://cmake.org/download/
- During installation, choose "Add CMake to system PATH"

Alternatively, CMake is included with Visual Studio if you installed the CMake component.

### 4. Verify Prerequisites

Open PowerShell and verify:

```powershell
# Check Visual Studio compiler
where cl.exe

# Check CMake
cmake --version

# Check Qt (replace with your Qt path)
dir C:\Qt\6.5.3\msvc2019_64\bin\qmake.exe
```

## Building NEXY Terminal

### Method 1: Command Line Build (Recommended)

#### Step 1: Open Visual Studio Developer Command Prompt

Open **"x64 Native Tools Command Prompt for VS 2019"** from the Start Menu.

This ensures the correct compiler environment is loaded.

#### Step 2: Navigate to Project Directory

```cmd
cd C:\path\to\NEXYTerminal
```

#### Step 3: Set Qt Environment Variable

Replace the path with your actual Qt installation:

```cmd
set Qt6_DIR=C:\Qt\6.5.3\msvc2019_64\lib\cmake\Qt6
```

#### Step 4: Configure the Project

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
```

For Visual Studio 2022, use:
```cmd
cmake .. -G "Visual Studio 17 2022" -A x64
```

#### Step 5: Build the Project

```cmd
cmake --build . --config Release
```

Or for Debug build:
```cmd
cmake --build . --config Debug
```

#### Step 6: Deploy Qt Dependencies

Navigate to the output directory and deploy Qt DLLs:

```cmd
cd bin\Release
C:\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe NEXYTerminal.exe
```

#### Step 7: Run the Application

```cmd
NEXYTerminal.exe
```

### Method 2: Qt Creator (Alternative)

#### Step 1: Open Qt Creator

Launch Qt Creator from the Start Menu or from your Qt installation folder.

#### Step 2: Open Project

1. Click **File → Open File or Project**
2. Navigate to `NEXYTerminal` folder
3. Select `CMakeLists.txt`
4. Click **Open**

#### Step 3: Configure Kits

1. Select your Qt MSVC kit (e.g., "Desktop Qt 6.5.3 MSVC2019 64bit")
2. Click **Configure Project**

#### Step 4: Build

1. Select **Release** build configuration from the bottom-left dropdown
2. Click **Build → Build Project "NEXYTerminal"** (or press Ctrl+B)

#### Step 5: Run

1. Click the green **Run** button (or press Ctrl+R)
2. NEXY Terminal will launch

### Method 3: Visual Studio with CMake Support

#### Step 1: Open Visual Studio

Launch Visual Studio 2019 or 2022.

#### Step 2: Open Folder

1. Click **File → Open → Folder**
2. Select the `NEXYTerminal` folder
3. Visual Studio will automatically detect `CMakeLists.txt`

#### Step 3: Configure CMake

1. Wait for CMake configuration to complete
2. If Qt is not found, add Qt path to `CMakeSettings.json`:

```json
{
  "configurations": [
    {
      "name": "x64-Release",
      "generator": "Ninja",
      "configurationType": "Release",
      "buildRoot": "${projectDir}\\build\\${name}",
      "cmakeCommandArgs": "-DQt6_DIR=C:/Qt/6.5.3/msvc2019_64/lib/cmake/Qt6"
    }
  ]
}
```

#### Step 4: Build

1. Click **Build → Build All** (or press Ctrl+Shift+B)
2. The executable will be in `build\x64-Release\bin\`

#### Step 5: Deploy and Run

1. Copy the executable to a separate folder
2. Run `windeployqt` as described in Method 1, Step 6
3. Run `NEXYTerminal.exe`

## Troubleshooting

### CMake Can't Find Qt

**Error**: `Could not find a package configuration file provided by "Qt6"`

**Solution**:
```cmd
set Qt6_DIR=C:\Qt\6.5.3\msvc2019_64\lib\cmake\Qt6
```

Or add to CMake command:
```cmd
cmake .. -DQt6_DIR=C:\Qt\6.5.3\msvc2019_64\lib\cmake\Qt6
```

### Missing DLL Errors

**Error**: `The code execution cannot proceed because Qt6Core.dll was not found`

**Solution**:
Run `windeployqt` in the executable directory:
```cmd
C:\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe NEXYTerminal.exe
```

### Compiler Not Found

**Error**: `The C++ compiler identification is unknown`

**Solution**:
- Use the Visual Studio Developer Command Prompt
- Or install "Desktop development with C++" in Visual Studio Installer

### Build Fails with Linker Errors

**Solution**:
- Ensure MSVC compiler matches Qt version (both 2019 or both 2022)
- Clean and rebuild:
```cmd
rd /s /q build
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### Application Won't Start

**Solution**:
1. Verify all Qt DLLs are present in the executable directory
2. Check Windows Event Viewer for error details
3. Run from Command Prompt to see error messages:
```cmd
cd build\bin\Release
NEXYTerminal.exe
```

## Creating a Distributable Package

### Step 1: Build Release Version

```cmd
cmake --build . --config Release
```

### Step 2: Create Distribution Folder

```cmd
mkdir NEXYTerminal-Distribution
cd NEXYTerminal-Distribution
copy ..\build\bin\Release\NEXYTerminal.exe .
```

### Step 3: Deploy Qt Dependencies

```cmd
C:\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe NEXYTerminal.exe
```

### Step 4: Add VC++ Runtime (if needed)

Copy the Visual C++ Runtime DLLs:
- `vcruntime140.dll`
- `msvcp140.dll`

These are typically in:
- `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Redist\MSVC\<version>\x64\Microsoft.VC142.CRT\`

Or bundle the VC++ Redistributable installer.

### Step 5: Package

Your distribution folder now contains:
- `NEXYTerminal.exe`
- Qt DLLs and plugins
- VC++ Runtime DLLs (optional if using redistributable)

Compress to ZIP or create an installer using tools like:
- Inno Setup
- NSIS
- WiX Toolset

## System Requirements

### Development Requirements
- Windows 10 or Windows 11
- Visual Studio 2019/2022 with C++ tools
- Qt 6.2 or later
- CMake 3.16 or later
- 4 GB RAM minimum
- 10 GB disk space for Qt and tools

### Runtime Requirements
- Windows 10 (1809 or later) or Windows 11
- Visual C++ Redistributable (if not bundled)
- ~50 MB disk space

## Next Steps

After successful build:

1. **Run the application**: `build\bin\Release\NEXYTerminal.exe`
2. **Configure settings**: Click ⚙ Settings to customize appearance
3. **Test functionality**: Try PowerShell and CMD commands
4. **Create multiple tabs**: Click + New Tab

## Getting Help

If you encounter issues:

1. Check the troubleshooting section above
2. Verify all prerequisites are installed correctly
3. Review the CMake output for specific errors
4. Ensure paths match your actual installation locations

## Build Time

Expected build times:
- First build: 2-5 minutes
- Incremental builds: 10-30 seconds

## Additional Resources

- Qt Documentation: https://doc.qt.io/qt-6/
- CMake Documentation: https://cmake.org/documentation/
- Visual Studio Documentation: https://docs.microsoft.com/visualstudio/

## License

This project is provided as-is for educational and development purposes.
