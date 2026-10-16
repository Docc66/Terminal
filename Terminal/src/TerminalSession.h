#ifndef TERMINALSESSION_H
#define TERMINALSESSION_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

class TerminalSession : public QObject {
    Q_OBJECT

public:
    enum ShellType {
        PowerShell,
        CommandPrompt
    };

    explicit TerminalSession(ShellType shell = PowerShell, QObject* parent = nullptr);
    ~TerminalSession();

    void start();
    void sendInput(const QString& input);
    void terminate();
    bool isRunning() const;
    QString currentDirectory() const { return m_currentDirectory; }

signals:
    void outputReceived(const QString& output);
    void errorReceived(const QString& error);
    void terminated();
    void directoryChanged(const QString& directory);

private slots:
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QProcess* m_process;
    ShellType m_shellType;
    QString m_currentDirectory;
    QString m_outputBuffer;
    
    void parseDirectoryFromOutput(const QString& output);
};

#endif
