#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QColor>
#include <QFont>

class Settings {
public:
    enum Theme {
        Dark,
        Light
    };

    enum CursorStyle {
        Block,
        Underline,
        Bar
    };

    enum DefaultShell {
        PowerShell,
        CommandPrompt
    };

    static Settings& instance();

    // Theme
    Theme theme() const { return m_theme; }
    void setTheme(Theme theme);

    // Font
    QFont font() const { return m_font; }
    void setFont(const QFont& font);

    // Colors
    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor& color);

    QColor textColor() const { return m_textColor; }
    void setTextColor(const QColor& color);

    // Cursor
    CursorStyle cursorStyle() const { return m_cursorStyle; }
    void setCursorStyle(CursorStyle style);

    // Shell
    DefaultShell defaultShell() const { return m_defaultShell; }
    void setDefaultShell(DefaultShell shell);

    // Scrollback
    int scrollbackLines() const { return m_scrollbackLines; }
    void setScrollbackLines(int lines);

    // Transparency
    int opacity() const { return m_opacity; }
    void setOpacity(int opacity);

    void load();
    void save();

private:
    Settings();
    ~Settings();
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    Theme m_theme;
    QFont m_font;
    QColor m_backgroundColor;
    QColor m_textColor;
    CursorStyle m_cursorStyle;
    DefaultShell m_defaultShell;
    int m_scrollbackLines;
    int m_opacity;
};

#endif // SETTINGS_H
