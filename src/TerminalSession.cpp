#include "TerminalSession.h"
#include <QDir>
#include <QRegularExpression>
#include <QDebug>

TerminalSession::TerminalSession(ShellType shell, QObject* parent)
    : QObject(parent)
    , m_process(new QProcess(this))
    , m_shellType(shell)
    , m_currentDirectory(QDir::currentPath())
{
    connect(m_process, &QProcess::readyReadStandardOutput,
            this, &TerminalSession::onReadyReadStandardOutput);
    connect(m_process, &QProcess::readyReadStandardError,
            this, &TerminalSession::onReadyReadStandardError);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &TerminalSession::onFinished);

    m_process->setWorkingDirectory(m_currentDirectory);
    m_process->setProcessChannelMode(QProcess::SeparateChannels);
}

TerminalSession::~TerminalSession() {
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        if (!m_process->waitForFinished(1000)) {
            m_process->kill();
        }
    }
}

void TerminalSession::start() {
    QString program;
    QStringList arguments;

    if (m_shellType == PowerShell) {
        program = "powershell.exe";
        arguments << "-NoLogo" << "-NoExit";
    } else {
        program = "cmd.exe";
        arguments << "/Q";
    }

    m_process->start(program, arguments);
    
    if (!m_process->waitForStarted()) {
        emit errorReceived("Failed to start shell process\r\n");
    }
}

void TerminalSession::sendInput(const QString& input) {
    if (m_process->state() == QProcess::Running) {
        QString command = input;
        if (!command.endsWith('\n')) {
            command += "\r\n";
        }
        m_process->write(command.toUtf8());
    }
}

void TerminalSession::terminate() {
    if (m_process->state() != QProcess::NotRunning) {
        m_process->terminate();
        if (!m_process->waitForFinished(1000)) {
            m_process->kill();
        }
    }
}

bool TerminalSession::isRunning() const {
    return m_process->state() == QProcess::Running;
}

void TerminalSession::onReadyReadStandardOutput() {
    QByteArray data = m_process->readAllStandardOutput();
    QString output = QString::fromLocal8Bit(data);
    
    m_outputBuffer += output;
    parseDirectoryFromOutput(m_outputBuffer);
    
    emit outputReceived(output);
}

void TerminalSession::onReadyReadStandardError() {
    QByteArray data = m_process->readAllStandardError();
    QString error = QString::fromLocal8Bit(data);
    emit errorReceived(error);
}

void TerminalSession::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit terminated();
}

void TerminalSession::parseDirectoryFromOutput(const QString& output) {
    // Try to detect directory changes in output
    // For PowerShell: Look for "PS drive:\path>"
    // For CMD: Look for "drive:\path>"
    
    QRegularExpression psPattern(R"(PS\s+([A-Z]:[^>]+)>)");
    QRegularExpression cmdPattern(R"(([A-Z]:\\[^>]+)>)");
    
    QRegularExpressionMatch match;
    
    if (m_shellType == PowerShell) {
        match = psPattern.match(output);
        if (match.hasMatch()) {
            QString newDir = match.captured(1).trimmed();
            if (newDir != m_currentDirectory && QDir(newDir).exists()) {
                m_currentDirectory = newDir;
                emit directoryChanged(m_currentDirectory);
            }
        }
    } else {
        match = cmdPattern.match(output);
        if (match.hasMatch()) {
            QString newDir = match.captured(1).trimmed();
            if (newDir != m_currentDirectory && QDir(newDir).exists()) {
                m_currentDirectory = newDir;
                emit directoryChanged(m_currentDirectory);
            }
        }
    }
}
