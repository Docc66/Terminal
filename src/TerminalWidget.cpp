#include "TerminalWidget.h"
#include "Settings.h"
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QTextCursor>
#include <QScrollBar>
#include <QApplication>
#include <QClipboard>

TerminalWidget::TerminalWidget(TerminalSession::ShellType shell, QWidget* parent)
    : QWidget(parent)
    , m_textEdit(new QTextEdit(this))
    , m_session(new TerminalSession(shell, this))
    , m_historyIndex(-1)
    , m_promptPosition(0)
    , m_cursorVisible(true)
    , m_cursorTimer(new QTimer(this))
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_textEdit);

    m_textEdit->setReadOnly(true);
    m_textEdit->setUndoRedoEnabled(false);
    m_textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
    m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_textEdit->setContextMenuPolicy(Qt::NoContextMenu);

    applySettings();

    connect(m_session, &TerminalSession::outputReceived,
            this, &TerminalWidget::onOutputReceived);
    connect(m_session, &TerminalSession::errorReceived,
            this, &TerminalWidget::onErrorReceived);
    connect(m_session, &TerminalSession::terminated,
            this, &TerminalWidget::onSessionTerminated);
    connect(m_session, &TerminalSession::directoryChanged,
            this, &TerminalWidget::onDirectoryChanged);

    connect(m_cursorTimer, &QTimer::timeout, this, &TerminalWidget::updateCursor);
    m_cursorTimer->start(530);

    m_session->start();
    m_currentDirectory = m_session->currentDirectory();
}

TerminalWidget::~TerminalWidget() {
}

void TerminalWidget::focusInput() {
    m_textEdit->setFocus();
}

QString TerminalWidget::currentDirectory() const {
    return m_currentDirectory;
}

bool TerminalWidget::isSessionRunning() const {
    return m_session->isRunning();
}

void TerminalWidget::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        executeCommand(m_currentInput);
        m_currentInput.clear();
    } else if (event->key() == Qt::Key_Backspace) {
        if (!m_currentInput.isEmpty()) {
            m_currentInput.chop(1);
            updatePrompt();
        }
    } else if (event->key() == Qt::Key_Up) {
        historyUp();
    } else if (event->key() == Qt::Key_Down) {
        historyDown();
    } else if (event->key() == Qt::Key_C && (event->modifiers() & Qt::ControlModifier)) {
        if (m_textEdit->textCursor().hasSelection()) {
            QApplication::clipboard()->setText(m_textEdit->textCursor().selectedText());
        } else {
            m_session->sendInput("\x03");
            m_currentInput.clear();
            updatePrompt();
        }
    } else if (event->key() == Qt::Key_V && (event->modifiers() & Qt::ControlModifier)) {
        QString text = QApplication::clipboard()->text();
        m_currentInput += text;
        updatePrompt();
    } else if (!event->text().isEmpty() && event->text()[0].isPrint()) {
        m_currentInput += event->text();
        updatePrompt();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void TerminalWidget::mousePressEvent(QMouseEvent* event) {
    m_textEdit->setFocus();
    QWidget::mousePressEvent(event);
}

void TerminalWidget::onOutputReceived(const QString& output) {
    appendOutput(output, Settings::instance().textColor());
}

void TerminalWidget::onErrorReceived(const QString& error) {
    appendOutput(error, QColor(220, 80, 80));
}

void TerminalWidget::onSessionTerminated() {
    appendOutput("\r\n[Process terminated]\r\n", QColor(150, 150, 150));
}

void TerminalWidget::onDirectoryChanged(const QString& directory) {
    m_currentDirectory = directory;
    emit directoryChanged(directory);
}

void TerminalWidget::updateCursor() {
    m_cursorVisible = !m_cursorVisible;
    updatePrompt();
}

void TerminalWidget::appendOutput(const QString& text, const QColor& color) {
    QTextCursor cursor(m_textEdit->document());
    cursor.movePosition(QTextCursor::End);
    
    cursor.select(QTextCursor::LineUnderCursor);
    QString currentLine = cursor.selectedText();
    if (currentLine.startsWith("$") || currentLine.startsWith(">")) {
        cursor.removeSelectedText();
    }
    
    cursor.movePosition(QTextCursor::End);
    
    QTextCharFormat format;
    format.setForeground(color.isValid() ? color : Settings::instance().textColor());
    format.setFont(Settings::instance().font());
    
    cursor.insertText(text, format);
    
    m_promptPosition = m_textEdit->document()->characterCount();
    
    m_textEdit->verticalScrollBar()->setValue(
        m_textEdit->verticalScrollBar()->maximum()
    );
    
    updatePrompt();
}

void TerminalWidget::executeCommand(const QString& command) {
    if (!command.trimmed().isEmpty()) {
        m_commandHistory.append(command);
        m_historyIndex = m_commandHistory.size();
        m_session->sendInput(command);
    } else {
        m_session->sendInput("\r\n");
    }
}

void TerminalWidget::historyUp() {
    if (m_historyIndex > 0 && !m_commandHistory.isEmpty()) {
        m_historyIndex--;
        m_currentInput = m_commandHistory[m_historyIndex];
        updatePrompt();
    }
}

void TerminalWidget::historyDown() {
    if (m_historyIndex < m_commandHistory.size() - 1) {
        m_historyIndex++;
        m_currentInput = m_commandHistory[m_historyIndex];
        updatePrompt();
    } else if (m_historyIndex == m_commandHistory.size() - 1) {
        m_historyIndex = m_commandHistory.size();
        m_currentInput.clear();
        updatePrompt();
    }
}

void TerminalWidget::applySettings() {
    const Settings& settings = Settings::instance();
    
    QString styleSheet = QString(
        "QTextEdit {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: none;"
        "  padding: 8px;"
        "}"
    ).arg(settings.backgroundColor().name())
     .arg(settings.textColor().name());
    
    m_textEdit->setStyleSheet(styleSheet);
    m_textEdit->setFont(settings.font());
}

void TerminalWidget::updatePrompt() {
    QTextCursor cursor(m_textEdit->document());
    cursor.movePosition(QTextCursor::End);
    
    QString text = m_textEdit->toPlainText();
    int lastNewline = text.lastIndexOf('\n');
    
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    QString currentLine = cursor.selectedText();
    
    if (currentLine.startsWith("$ ") || currentLine.startsWith("> ")) {
        cursor.removeSelectedText();
    }
    
    cursor.movePosition(QTextCursor::End);
    QTextCharFormat format;
    format.setForeground(Settings::instance().textColor());
    format.setFont(Settings::instance().font());
    
    QString prompt = getPromptText();
    QString cursorChar = m_cursorVisible ? "█" : " ";
    cursor.insertText(prompt + m_currentInput + cursorChar, format);
    
    m_textEdit->verticalScrollBar()->setValue(
        m_textEdit->verticalScrollBar()->maximum()
    );
}

QString TerminalWidget::getPromptText() const {
    return "$ ";
}
