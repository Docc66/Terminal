#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QString>
#include <QStringList>
#include <QTimer>
#include "TerminalSession.h"

class TerminalWidget : public QWidget {
    Q_OBJECT

public:
    explicit TerminalWidget(TerminalSession::ShellType shell = TerminalSession::PowerShell,
                           QWidget* parent = nullptr);
    ~TerminalWidget();

    void focusInput();
    QString currentDirectory() const;
    bool isSessionRunning() const;

signals:
    void titleChanged(const QString& title);
    void directoryChanged(const QString& directory);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void onOutputReceived(const QString& output);
    void onErrorReceived(const QString& error);
    void onSessionTerminated();
    void onDirectoryChanged(const QString& directory);
    void updateCursor();

private:
    QTextEdit* m_textEdit;
    TerminalSession* m_session;
    QString m_currentInput;
    QStringList m_commandHistory;
    int m_historyIndex;
    int m_promptPosition;
    bool m_cursorVisible;
    QTimer* m_cursorTimer;
    QString m_currentDirectory;

    void appendOutput(const QString& text, const QColor& color = QColor());
    void executeCommand(const QString& command);
    void historyUp();
    void historyDown();
    void applySettings();
    void updatePrompt();
    QString getPromptText() const;
};

#endif // TERMINALWIDGET_H
