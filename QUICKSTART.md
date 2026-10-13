# NEXY Terminal - Quick Start Guide

Get NEXY Terminal running in 5 minutes!

## ⚡ Fast Track Installation

### Prerequisites Check
- ✅ Windows 10/11
- ✅ Visual Studio 2019 or 2022 installed
- ✅ Qt 6 installed

### Build in 5 Steps

#### 1️⃣ Open Visual Studio Developer Command Prompt
Search in Start Menu: "x64 Native Tools Command Prompt for VS 2019"

#### 2️⃣ Navigate to Project
```cmd
cd C:\path\to\NEXYTerminal
```

#### 3️⃣ Set Qt Path
```cmd
set Qt6_DIR=C:\Qt\6.5.3\msvc2019_64\lib\cmake\Qt6
```
*(Replace with your Qt installation path)*

#### 4️⃣ Build
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

#### 5️⃣ Deploy and Run
```cmd
cd bin\Release
C:\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe NEXYTerminal.exe
NEXYTerminal.exe
```

🎉 **Done!** NEXY Terminal should now be running.

---

## 📦 Don't Have Qt or Visual Studio?

### Install Visual Studio (Free)
1. Download: https://visualstudio.microsoft.com/downloads/
2. Install "Community" edition (free)
3. Select: "Desktop development with C++"
4. Install time: ~15-20 minutes

### Install Qt 6 (Free)
1. Download: https://www.qt.io/download-qt-installer
2. Create free account
3. Select: Qt 6.5 → MSVC 2019 64-bit
4. Install time: ~10-15 minutes

---

## 🎯 Quick Commands Reference

### Basic Usage
- **New Tab**: `Ctrl + T`
- **Close Tab**: `Ctrl + W`
- **Settings**: `Ctrl + ,`
- **Copy**: `Ctrl + C` (with selection)
- **Paste**: `Ctrl + V`
- **History**: `↑` / `↓` arrow keys

### First Time Setup
1. Launch NEXY Terminal
2. Press `Ctrl + ,` for Settings
3. Customize:
   - Choose your preferred font
   - Adjust font size
   - Select default shell (PowerShell/CMD)

---

## ❓ Troubleshooting Quick Fixes

### "Qt6Core.dll not found"
```cmd
cd build\bin\Release
C:\Qt\6.5.3\msvc2019_64\bin\windeployqt.exe NEXYTerminal.exe
```

### "CMake can't find Qt"
Add to your cmake command:
```cmd
cmake .. -DQt6_DIR=C:\Qt\6.5.3\msvc2019_64\lib\cmake\Qt6
```

### "Compiler not found"
Use Visual Studio Developer Command Prompt (not regular CMD or PowerShell)

---

## 📁 Project Structure
```
NEXYTerminal/
├── src/                    # Source code
│   ├── main.cpp           # Entry point
│   ├── MainWindow.*       # Main window
│   ├── TerminalWidget.*   # Terminal display
│   ├── TerminalSession.*  # Shell integration
│   ├── SettingsDialog.*   # Settings UI
│   └── Settings.*         # Settings manager
├── resources/             # Qt resources
├── build/                 # Build output (created)
├── CMakeLists.txt        # Build configuration
├── README.md             # Full documentation
├── BUILD.md              # Detailed build guide
└── QUICKSTART.md         # This file
```

---

## 🚀 What's Next?

### Try These Features
1. **Multiple Tabs**: Click "+ New Tab" to open multiple terminals
2. **PowerShell Commands**: Try `Get-ChildItem`, `cd`, `dir`
3. **CMD Commands**: Try `dir`, `cd`, `ipconfig`
4. **Customization**: Play with themes and colors in Settings
5. **Command History**: Use arrow keys to recall previous commands

### Customize Your Terminal
- **Font**: Try "Consolas", "Cascadia Code", or "Fira Code"
- **Size**: 10-12pt works well for most displays
- **Opacity**: Lower opacity for a modern look (but keep it readable!)
- **Colors**: Experiment with background and text colors

---

## 📖 More Information

- **Full Build Guide**: See `BUILD.md`
- **Complete Documentation**: See `README.md`
- **Features & Architecture**: See `README.md` sections

---

## 🎨 Design Philosophy

NEXY Terminal follows these principles:
- **Minimal**: Clean, no clutter
- **Professional**: Polished and stable
- **Functional**: Real shell integration
- **Fast**: Responsive and lightweight

---

## ⚙️ System Requirements

**Minimum**:
- Windows 10 (1809+) or Windows 11
- 2 GB RAM
- 100 MB disk space

**Recommended**:
- Windows 11
- 4 GB RAM
- SSD storage

---

## 🐛 Found a Bug?

If something isn't working:
1. Check `BUILD.md` troubleshooting section
2. Verify all prerequisites are installed
3. Try a clean rebuild:
   ```cmd
   rd /s /q build
   mkdir build
   cd build
   cmake .. && cmake --build . --config Release
   ```

---

## 💡 Tips

- Use Visual Studio Developer Command Prompt for building
- Run `windeployqt` after every build before running the exe
- Keep Qt and Visual Studio versions consistent (both 2019 or both 2022)
- Settings are saved automatically in Windows Registry

---

**Happy terminal-ing!** 🖥️✨
