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

    Theme theme() const { return m_theme; }
    void setTheme(Theme theme);

    QFont font() const { return m_font; }
    void setFont(const QFont& font);

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor& color);

    QColor textColor() const { return m_textColor; }
    void setTextColor(const QColor& color);

    CursorStyle cursorStyle() const { return m_cursorStyle; }
    void setCursorStyle(CursorStyle style);

    DefaultShell defaultShell() const { return m_defaultShell; }
    void setDefaultShell(DefaultShell shell);

    int scrollbackLines() const { return m_scrollbackLines; }
    void setScrollbackLines(int lines);

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

#endif
