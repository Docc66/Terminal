#include "Settings.h"
#include <QSettings>
#include <QApplication>

Settings& Settings::instance() {
    static Settings instance;
    return instance;
}

Settings::Settings()
    : m_theme(Dark)
    , m_font("Consolas", 10)
    , m_backgroundColor(11, 11, 13)
    , m_textColor(220, 220, 220)
    , m_cursorStyle(Block)
    , m_defaultShell(PowerShell)
    , m_scrollbackLines(10000)
    , m_opacity(100)
{
    load();
}

Settings::~Settings() {
    save();
}

void Settings::setTheme(Theme theme) {
    m_theme = theme;
    if (theme == Light) {
        m_backgroundColor = QColor(250, 250, 250);
        m_textColor = QColor(30, 30, 30);
    } else {
        m_backgroundColor = QColor(11, 11, 13);
        m_textColor = QColor(220, 220, 220);
    }
}

void Settings::setFont(const QFont& font) {
    m_font = font;
}

void Settings::setBackgroundColor(const QColor& color) {
    m_backgroundColor = color;
}

void Settings::setTextColor(const QColor& color) {
    m_textColor = color;
}

void Settings::setCursorStyle(CursorStyle style) {
    m_cursorStyle = style;
}

void Settings::setDefaultShell(DefaultShell shell) {
    m_defaultShell = shell;
}

void Settings::setScrollbackLines(int lines) {
    m_scrollbackLines = lines;
}

void Settings::setOpacity(int opacity) {
    m_opacity = qBound(50, opacity, 100);
}

void Settings::load() {
    QSettings settings("NEXY", "NEXYTerminal");
    
    m_theme = static_cast<Theme>(settings.value("theme", Dark).toInt());
    
    QString fontFamily = settings.value("fontFamily", "Consolas").toString();
    int fontSize = settings.value("fontSize", 10).toInt();
    m_font = QFont(fontFamily, fontSize);
    
    m_backgroundColor = settings.value("backgroundColor", QColor(11, 11, 13)).value<QColor>();
    m_textColor = settings.value("textColor", QColor(220, 220, 220)).value<QColor>();
    m_cursorStyle = static_cast<CursorStyle>(settings.value("cursorStyle", Block).toInt());
    m_defaultShell = static_cast<DefaultShell>(settings.value("defaultShell", PowerShell).toInt());
    m_scrollbackLines = settings.value("scrollbackLines", 10000).toInt();
    m_opacity = settings.value("opacity", 100).toInt();
}

void Settings::save() {
    QSettings settings("NEXY", "NEXYTerminal");
    
    settings.setValue("theme", m_theme);
    settings.setValue("fontFamily", m_font.family());
    settings.setValue("fontSize", m_font.pointSize());
    settings.setValue("backgroundColor", m_backgroundColor);
    settings.setValue("textColor", m_textColor);
    settings.setValue("cursorStyle", m_cursorStyle);
    settings.setValue("defaultShell", m_defaultShell);
    settings.setValue("scrollbackLines", m_scrollbackLines);
    settings.setValue("opacity", m_opacity);
}
