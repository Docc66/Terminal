#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include "TerminalWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onNewTab();
    void onCloseTab(int index);
    void onTabChanged(int index);
    void onSettings();
    void onSettingsChanged();
    void updateStatusBar();

private:
    QTabWidget* m_tabWidget;
    QToolBar* m_toolBar;
    QStatusBar* m_statusBar;
    QLabel* m_directoryLabel;
    
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void applyTheme();
    void addNewTerminal(TerminalSession::ShellType shell);
};

#endif // MAINWINDOW_H
