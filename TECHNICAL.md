# NEXY Terminal - Technical Documentation

## 🔧 Technical Architecture

### System Overview

NEXY Terminal is a native Windows desktop application built with Qt 6 framework and C++17. It uses Qt's signal-slot mechanism for event-driven communication between components and QProcess for shell integration.

## 📐 Architecture Diagram

```
┌──────────────────────────────────────────────────────────┐
│                     Operating System                      │
│                    (Windows 10/11)                        │
└──────────────────────────────────────────────────────────┘
                            ▲
                            │ Win32 API / Qt Platform Abstraction
                            ▼
┌──────────────────────────────────────────────────────────┐
│                      Qt 6 Framework                       │
│  ┌─────────────────────────────────────────────────┐    │
│  │ Qt Widgets (GUI)   Qt Core (Base)   Qt Gui      │    │
│  └─────────────────────────────────────────────────┘    │
└──────────────────────────────────────────────────────────┘
                            ▲
                            │ Qt API
                            ▼
┌──────────────────────────────────────────────────────────┐
│                  NEXY Terminal Application                │
│                                                           │
│  ┌────────────────────────────────────────────────┐     │
│  │              MainWindow (QMainWindow)          │     │
│  │  ┌──────────────────────────────────────────┐ │     │
│  │  │  MenuBar  │  ToolBar  │  StatusBar       │ │     │
│  │  └──────────────────────────────────────────┘ │     │
│  │  ┌──────────────────────────────────────────┐ │     │
│  │  │         QTabWidget                       │ │     │
│  │  │  ┌────────┐  ┌────────┐  ┌────────┐    │ │     │
│  │  │  │ Term 1 │  │ Term 2 │  │ Term 3 │    │ │     │
│  │  │  └────────┘  └────────┘  └────────┘    │ │     │
│  │  └──────────────────────────────────────────┘ │     │
│  └────────────────────────────────────────────────┘     │
│                                                           │
│  ┌────────────────────────────────────────────────┐     │
│  │       TerminalWidget (QWidget)                 │     │
│  │  ┌──────────────────────────────────────────┐ │     │
│  │  │         QTextEdit (Display)              │ │     │
│  │  └──────────────────────────────────────────┘ │     │
│  │  ┌──────────────────────────────────────────┐ │     │
│  │  │     TerminalSession (QObject)            │ │     │
│  │  │  ┌────────────────────────────────────┐  │ │     │
│  │  │  │  QProcess (Shell Process)          │  │ │     │
│  │  │  └────────────────────────────────────┘  │ │     │
│  │  └──────────────────────────────────────────┘ │     │
│  └────────────────────────────────────────────────┘     │
│                                                           │
│  ┌────────────────────────────────────────────────┐     │
│  │     Settings (Singleton)                       │     │
│  │  ┌──────────────────────────────────────────┐ │     │
│  │  │  QSettings (Registry Storage)            │ │     │
│  │  └──────────────────────────────────────────┘ │     │
│  └────────────────────────────────────────────────┘     │
└──────────────────────────────────────────────────────────┘
                            ▲
                            │ QProcess
                            ▼
┌──────────────────────────────────────────────────────────┐
│              Shell Processes                              │
│  ┌─────────────────┐         ┌──────────────────┐       │
│  │ powershell.exe  │         │    cmd.exe       │       │
│  └─────────────────┘         └──────────────────┘       │
└──────────────────────────────────────────────────────────┘
```

## 🔍 Component Details

### 1. MainWindow

**File**: `MainWindow.h`, `MainWindow.cpp`

**Inheritance**: `QMainWindow`

**Purpose**: Root window of the application, container for all UI elements.

**Key Members**:
```cpp
class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    QTabWidget* m_tabWidget;      // Container for terminal tabs
    QToolBar* m_toolBar;          // Top toolbar
    QStatusBar* m_statusBar;      // Bottom status bar
    QLabel* m_directoryLabel;     // Current directory indicator
};
```

**Responsibilities**:
- Window lifecycle management
- Menu and toolbar creation
- Tab management (add, remove, switch)
- Theme application
- Settings dialog coordination
- Status bar updates

**Signals**: None (uses internal slots)

**Slots**:
```cpp
void onNewTab();           // Creates new terminal tab
void onCloseTab(int);      // Closes specified tab
void onTabChanged(int);    // Handles tab switch
void onSettings();         // Opens settings dialog
void onSettingsChanged();  // Applies new settings
void updateStatusBar();    // Updates directory display
```

**Event Handlers**:
```cpp
void closeEvent(QCloseEvent*);  // Handles window close with confirmation
```

### 2. TerminalWidget

**File**: `TerminalWidget.h`, `TerminalWidget.cpp`

**Inheritance**: `QWidget`

**Purpose**: Terminal display and input handling for a single session.

**Key Members**:
```cpp
class TerminalWidget : public QWidget {
    Q_OBJECT
    
private:
    QTextEdit* m_textEdit;              // Text display area
    TerminalSession* m_session;         // Shell process manager
    QString m_currentInput;             // Current line input
    QStringList m_commandHistory;       // Previous commands
    int m_historyIndex;                 // Position in history
    int m_promptPosition;               // Cursor position in document
    bool m_cursorVisible;               // Cursor blink state
    QTimer* m_cursorTimer;              // Cursor blink timer
    QString m_currentDirectory;         // Working directory
};
```

**Responsibilities**:
- Keyboard event processing
- Command history management
- Text rendering and formatting
- Cursor animation
- Copy/paste operations
- Prompt display

**Signals**:
```cpp
void titleChanged(const QString&);      // Emitted when tab title should change
void directoryChanged(const QString&);  // Emitted when directory changes
```

**Slots**:
```cpp
void onOutputReceived(const QString&);     // Handles stdout from shell
void onErrorReceived(const QString&);      // Handles stderr from shell
void onSessionTerminated();                // Handles shell exit
void onDirectoryChanged(const QString&);   // Updates directory display
void updateCursor();                       // Toggles cursor visibility
```

**Event Handlers**:
```cpp
void keyPressEvent(QKeyEvent*);      // Processes keyboard input
void mousePressEvent(QMouseEvent*);  // Handles mouse clicks
```

### 3. TerminalSession

**File**: `TerminalSession.h`, `TerminalSession.cpp`

**Inheritance**: `QObject`

**Purpose**: Shell process management and I/O handling.

**Key Members**:
```cpp
class TerminalSession : public QObject {
    Q_OBJECT
    
public:
    enum ShellType {
        PowerShell,
        CommandPrompt
    };
    
private:
    QProcess* m_process;              // Shell process
    ShellType m_shellType;            // Current shell type
    QString m_currentDirectory;       // Detected working directory
    QString m_outputBuffer;           // Accumulated output for parsing
};
```

**Responsibilities**:
- Process lifecycle (start, terminate)
- stdin/stdout/stderr handling
- Directory detection from output
- Shell type management

**Signals**:
```cpp
void outputReceived(const QString&);       // stdout data available
void errorReceived(const QString&);        // stderr data available
void terminated();                         // Process exited
void directoryChanged(const QString&);     // Working directory changed
```

**Slots**:
```cpp
void onReadyReadStandardOutput();    // Reads stdout from process
void onReadyReadStandardError();     // Reads stderr from process
void onFinished(int, QProcess::ExitStatus);  // Handles process exit
```

**Methods**:
```cpp
void start();                        // Starts shell process
void sendInput(const QString&);      // Sends command to shell
void terminate();                    // Stops shell process
bool isRunning() const;              // Checks process state
QString currentDirectory() const;    // Returns detected directory
```

### 4. Settings

**File**: `Settings.h`, `Settings.cpp`

**Inheritance**: None (Singleton)

**Purpose**: Application configuration management.

**Key Members**:
```cpp
class Settings {
public:
    enum Theme { Dark, Light };
    enum CursorStyle { Block, Underline, Bar };
    enum DefaultShell { PowerShell, CommandPrompt };
    
    static Settings& instance();  // Singleton accessor
    
private:
    Theme m_theme;
    QFont m_font;
    QColor m_backgroundColor;
    QColor m_textColor;
    CursorStyle m_cursorStyle;
    DefaultShell m_defaultShell;
    int m_scrollbackLines;
    int m_opacity;
};
```

**Storage**: Windows Registry (`HKEY_CURRENT_USER\Software\NEXY\NEXYTerminal`)

**Methods**:
```cpp
// Getters
Theme theme() const;
QFont font() const;
QColor backgroundColor() const;
QColor textColor() const;
CursorStyle cursorStyle() const;
DefaultShell defaultShell() const;
int scrollbackLines() const;
int opacity() const;

// Setters
void setTheme(Theme);
void setFont(const QFont&);
void setBackgroundColor(const QColor&);
void setTextColor(const QColor&);
void setCursorStyle(CursorStyle);
void setDefaultShell(DefaultShell);
void setScrollbackLines(int);
void setOpacity(int);

// Persistence
void load();
void save();
```

### 5. SettingsDialog

**File**: `SettingsDialog.h`, `SettingsDialog.cpp`

**Inheritance**: `QDialog`

**Purpose**: User interface for settings customization.

**Key Members**:
```cpp
class SettingsDialog : public QDialog {
    Q_OBJECT
    
private:
    QComboBox* m_themeCombo;
    QFontComboBox* m_fontCombo;
    QSpinBox* m_fontSizeSpinBox;
    QPushButton* m_backgroundColorButton;
    QPushButton* m_textColorButton;
    QComboBox* m_cursorStyleCombo;
    QComboBox* m_defaultShellCombo;
    QSpinBox* m_scrollbackSpinBox;
    QSlider* m_opacitySlider;
    QLabel* m_opacityLabel;
    
    QColor m_selectedBackgroundColor;
    QColor m_selectedTextColor;
};
```

**Signals**:
```cpp
void settingsChanged();  // Emitted when settings are applied
```

**Slots**:
```cpp
void onApply();                    // Applies settings without closing
void onOk();                       // Applies settings and closes
void onCancel();                   // Closes without applying
void onBackgroundColorClicked();   // Opens color picker for background
void onTextColorClicked();         // Opens color picker for text
```

## 🔄 Data Flow

### Command Execution Flow

```
1. User types in terminal
   ↓
2. TerminalWidget::keyPressEvent()
   - Accumulates input in m_currentInput
   ↓
3. User presses Enter
   ↓
4. TerminalWidget::executeCommand()
   - Adds to command history
   - Clears current input
   ↓
5. TerminalSession::sendInput()
   - Writes to QProcess stdin
   ↓
6. Shell process receives input
   - Executes command
   - Writes to stdout/stderr
   ↓
7. QProcess emits readyRead signals
   ↓
8. TerminalSession slots receive data
   - onReadyReadStandardOutput()
   - onReadyReadStandardError()
   ↓
9. Emits outputReceived() or errorReceived()
   ↓
10. TerminalWidget receives output
    ↓
11. TerminalWidget::appendOutput()
    - Formats text with color
    - Appends to QTextEdit
    - Scrolls to bottom
    - Updates prompt
```

### Settings Change Flow

```
1. User opens Settings (Ctrl+,)
   ↓
2. MainWindow::onSettings()
   - Creates SettingsDialog
   - Connects settingsChanged signal
   ↓
3. User modifies settings
   ↓
4. User clicks Apply or OK
   ↓
5. SettingsDialog::saveSettings()
   - Updates Settings singleton
   - Calls Settings::save()
   ↓
6. Emits settingsChanged()
   ↓
7. MainWindow::onSettingsChanged()
   - Calls applyTheme()
   - Updates all terminal widgets
   ↓
8. TerminalWidgets update appearance
   - Read new settings
   - Apply styles
   - Refresh display
```

## 🧵 Threading Model

**Main Thread**:
- UI rendering (Qt Widgets)
- Event processing
- User input handling

**QProcess Threads**:
- Managed internally by Qt
- I/O operations with shell processes
- Signals delivered to main thread via Qt's event system

**No Explicit Threading**:
- Application is single-threaded from developer perspective
- Qt handles threading internally for I/O
- All UI operations on main thread

## 💾 Memory Management

### Qt Parent-Child Memory Model

```cpp
// MainWindow owns tabWidget
QTabWidget* m_tabWidget = new QTabWidget(this);
                                          ↑
                                       parent

// Deleted automatically when MainWindow is destroyed
```

**Ownership Rules**:
1. QObject-derived classes with parent → parent deletes child
2. Top-level windows (MainWindow) → deleted when closed
3. QProcess → deleted when TerminalSession is deleted
4. Widgets added to layouts → layout takes ownership

### Resource Cleanup

**Process Termination**:
```cpp
TerminalSession::~TerminalSession() {
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();  // Try graceful shutdown
        if (!m_process->waitForFinished(1000)) {
            m_process->kill();   // Force kill if needed
        }
    }
}
```

**Tab Closure**:
```cpp
void MainWindow::onCloseTab(int index) {
    QWidget* widget = m_tabWidget->widget(index);
    m_tabWidget->removeTab(index);
    widget->deleteLater();  // Safe deletion via event loop
}
```

## 🎨 Styling System

### QSS (Qt Style Sheets)

NEXY Terminal uses Qt Style Sheets (QSS) for visual styling:

```cpp
QString styleSheet = QString(
    "QMainWindow {"
    "  background-color: %1;"
    "}"
    "QMenuBar {"
    "  background-color: %1;"
    "  color: %2;"
    "  border-bottom: 1px solid %3;"
    "}"
    // ... more styles
).arg(bgColor.name())
 .arg(textColor.name())
 .arg(borderColor.name());

setStyleSheet(styleSheet);
```

**Dynamic Styling**:
- Colors computed from Settings
- Applied at startup and after settings change
- Cascades to child widgets

## 🔐 Security Considerations

### Process Execution

**Input Sanitization**:
- All user input passed directly to shell
- Shell responsible for parsing and validation
- No command injection prevention needed (shell handles it)

**Process Isolation**:
- Each tab runs independent process
- No shared state between tabs
- Process death doesn't affect other tabs

### Settings Storage

**Registry Location**:
- User-specific hive (HKCU)
- No system-wide settings
- No elevation required

**Sensitive Data**:
- No passwords or secrets stored
- Only UI preferences saved

## 📊 Performance Optimization

### Output Buffering

```cpp
void TerminalSession::onReadyReadStandardOutput() {
    QByteArray data = m_process->readAllStandardOutput();
    QString output = QString::fromLocal8Bit(data);
    
    m_outputBuffer += output;  // Accumulate for parsing
    emit outputReceived(output);  // Emit immediately for display
}
```

**Strategy**: Emit output immediately for responsiveness, buffer for parsing.

### Text Rendering

**QTextEdit Optimization**:
- Hardware-accelerated rendering via Qt
- Automatic viewport culling (only visible text rendered)
- Incremental updates (append operations)

**Scrollback Management**:
```cpp
// Future enhancement: Limit document size
if (m_textEdit->document()->lineCount() > m_scrollbackLines) {
    // Remove old lines from top
}
```

### Memory Usage

**Per-Tab Overhead**:
- TerminalWidget: ~1-2 KB
- TerminalSession: ~1-2 KB
- QProcess: ~500 bytes
- QTextEdit document: ~1 KB per 100 lines

**Typical Memory**:
- 3 tabs with 1000 lines each: ~50 MB total
- Mostly Qt framework overhead

## 🧪 Testing Strategy

### Unit Testing (Recommended Addition)

```cpp
// Example test structure
class TestTerminalSession : public QObject {
    Q_OBJECT
    
private slots:
    void testProcessStart();
    void testCommandExecution();
    void testDirectoryDetection();
    void testProcessTermination();
};
```

### Integration Testing

**Manual Test Cases**:
1. Launch application
2. Create multiple tabs
3. Execute commands in each tab
4. Change settings
5. Close tabs
6. Exit application

### Stress Testing

**High Load Scenarios**:
- 10+ simultaneous tabs
- Commands producing 10,000+ lines of output
- Rapid command execution
- Long-running processes

## 🔌 Extension Points

### Future Plugin System

**Potential Architecture**:
```cpp
class ITerminalPlugin {
public:
    virtual ~ITerminalPlugin() = default;
    virtual QString name() const = 0;
    virtual void initialize(MainWindow*) = 0;
    virtual void shutdown() = 0;
};

// Plugin manager
class PluginManager {
public:
    void loadPlugin(const QString& path);
    void unloadPlugin(const QString& name);
private:
    QList<ITerminalPlugin*> m_plugins;
};
```

### Custom Themes

**Theme File Format** (JSON example):
```json
{
    "name": "Solarized Dark",
    "background": "#002b36",
    "foreground": "#839496",
    "cursor": "#93a1a1",
    "black": "#073642",
    "red": "#dc322f",
    "green": "#859900",
    // ... more colors
}
```

## 📈 Metrics & Monitoring

### Performance Metrics

**Measurable Values**:
- Startup time: Time from launch to first window
- Input latency: Keypress to display
- Render latency: Output received to displayed
- Memory per tab: RSS increase per new tab

**Profiling Tools**:
- Qt Creator Profiler
- Visual Studio Performance Profiler
- Windows Performance Monitor

## 🛠️ Build System Details

### CMake Configuration

**Key Variables**:
```cmake
CMAKE_CXX_STANDARD          = 17
CMAKE_AUTOMOC               = ON    # Auto-generate MOC files
CMAKE_AUTORCC               = ON    # Auto-process .qrc files
CMAKE_AUTOUIC               = ON    # Auto-process .ui files
```

**Build Targets**:
- `NEXYTerminal`: Main executable
- All: Build everything (default)

**Output Locations**:
```
build/
  ├── bin/
  │   ├── Debug/
  │   │   └── NEXYTerminal.exe
  │   └── Release/
  │       └── NEXYTerminal.exe
  └── CMakeFiles/
```

### Compiler Options

**MSVC Flags** (set by CMake):
- `/W3`: Warning level 3
- `/EHsc`: Enable C++ exception handling
- `/DUNICODE /D_UNICODE`: Unicode support
- `/MD` or `/MT`: Runtime library linking

## 📝 Code Conventions

### Naming

**Classes**: PascalCase (`MainWindow`, `TerminalWidget`)

**Methods**: camelCase (`executeCommand`, `updatePrompt`)

**Member Variables**: m_ prefix (`m_textEdit`, `m_session`)

**Constants**: ALL_CAPS or k prefix (`MAX_TABS` or `kDefaultPort`)

### File Organization

**Header Files** (.h):
- Include guards: `#ifndef FILENAME_H`
- Qt includes first
- Standard library includes second
- Project includes last

**Source Files** (.cpp):
- Corresponding header first
- Qt includes
- Standard library includes
- Other project includes

### Qt Conventions

**Signals**: No implementation, just declaration

**Slots**: Prefix with `on` (`onButtonClicked`)

**Q_OBJECT**: Required for signals/slots

**Getters**: `propertyName()` (no `get` prefix)

**Setters**: `setPropertyName()`

## 🔗 Dependencies

### Qt Modules

```
Qt6::Core       - Base classes, signals/slots, containers
Qt6::Widgets    - GUI components
Qt6::Gui        - Graphics, fonts, colors
```

### System Dependencies

```
Windows API     - Process management (via Qt)
MSVC Runtime    - C++ standard library
GDI+            - Graphics rendering (via Qt)
```

## 📚 References

### Qt Documentation
- QMainWindow: https://doc.qt.io/qt-6/qmainwindow.html
- QProcess: https://doc.qt.io/qt-6/qprocess.html
- QTextEdit: https://doc.qt.io/qt-6/qtextedit.html
- QSettings: https://doc.qt.io/qt-6/qsettings.html

### C++ Standards
- C++17 Features: https://en.cppreference.com/w/cpp/17

### CMake
- CMake Documentation: https://cmake.org/documentation/

---

**Document Version**: 1.0

**Last Updated**: 2024

**Target Audience**: Developers extending or maintaining NEXY Terminal
