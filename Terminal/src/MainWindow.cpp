#include "MainWindow.h"
#include "SettingsDialog.h"
#include "Settings.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_tabWidget(new QTabWidget(this))
    , m_directoryLabel(new QLabel(this))
{
    setWindowTitle("NEXY Terminal");
    resize(1000, 650);
    
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    applyTheme();
    
    addNewTerminal(static_cast<TerminalSession::ShellType>(
        Settings::instance().defaultShell()));
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    setCentralWidget(m_tabWidget);
    
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    m_tabWidget->setDocumentMode(true);
    
    connect(m_tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::onCloseTab);
    connect(m_tabWidget, &QTabWidget::currentChanged,
            this, &MainWindow::onTabChanged);
}

void MainWindow::setupMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu("&File");
    
    QAction* newTabAction = fileMenu->addAction("&New Tab");
    newTabAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(newTabAction, &QAction::triggered, this, &MainWindow::onNewTab);
    
    QAction* closeTabAction = fileMenu->addAction("&Close Tab");
    closeTabAction->setShortcut(QKeySequence("Ctrl+W"));
    connect(closeTabAction, &QAction::triggered, [this]() {
        if (m_tabWidget->count() > 0) {
            onCloseTab(m_tabWidget->currentIndex());
        }
    });
    
    fileMenu->addSeparator();
    
    QAction* settingsAction = fileMenu->addAction("&Settings");
    settingsAction->setShortcut(QKeySequence("Ctrl+,"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettings);
    
    fileMenu->addSeparator();
    
    QAction* exitAction = fileMenu->addAction("E&xit");
    exitAction->setShortcut(QKeySequence("Alt+F4"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
    
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    
    QAction* copyAction = editMenu->addAction("&Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    
    QAction* pasteAction = editMenu->addAction("&Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    
    QMenu* viewMenu = menuBar()->addMenu("&View");
    
    QAction* zoomInAction = viewMenu->addAction("Zoom &In");
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    
    QAction* zoomOutAction = viewMenu->addAction("Zoom &Out");
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    
    QAction* resetZoomAction = viewMenu->addAction("&Reset Zoom");
    resetZoomAction->setShortcut(QKeySequence("Ctrl+0"));
}

void MainWindow::setupToolBar() {
    m_toolBar = addToolBar("Main Toolbar");
    m_toolBar->setMovable(false);
    m_toolBar->setFloatable(false);
    
    QAction* newTabAction = m_toolBar->addAction("+ New Tab");
    connect(newTabAction, &QAction::triggered, this, &MainWindow::onNewTab);
    
    m_toolBar->addSeparator();
    
    QAction* settingsAction = m_toolBar->addAction("⚙ Settings");
    connect(settingsAction, &QAction::triggered, this, &MainWindow::onSettings);
}

void MainWindow::setupStatusBar() {
    m_statusBar = statusBar();
    m_statusBar->addPermanentWidget(m_directoryLabel);
    updateStatusBar();
}

void MainWindow::applyTheme() {
    const Settings& settings = Settings::instance();
    
    QColor bgColor = settings.backgroundColor();
    QColor textColor = settings.textColor();
    QColor borderColor = bgColor.lighter(120);
    
    QString styleSheet = QString(
        "QMainWindow {"
        "  background-color: %1;"
        "}"
        "QMenuBar {"
        "  background-color: %1;"
        "  color: %2;"
        "  border-bottom: 1px solid %3;"
        "  padding: 4px;"
        "}"
        "QMenuBar::item {"
        "  background-color: transparent;"
        "  padding: 4px 12px;"
        "  border-radius: 3px;"
        "}"
        "QMenuBar::item:selected {"
        "  background-color: %3;"
        "}"
        "QMenu {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "}"
        "QMenu::item {"
        "  padding: 6px 24px;"
        "}"
        "QMenu::item:selected {"
        "  background-color: %3;"
        "}"
        "QToolBar {"
        "  background-color: %1;"
        "  border-bottom: 1px solid %3;"
        "  spacing: 8px;"
        "  padding: 6px;"
        "}"
        "QToolButton {"
        "  background-color: transparent;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-radius: 4px;"
        "  padding: 6px 12px;"
        "}"
        "QToolButton:hover {"
        "  background-color: %3;"
        "}"
        "QTabWidget::pane {"
        "  border: none;"
        "  background-color: %1;"
        "}"
        "QTabBar::tab {"
        "  background-color: %1;"
        "  color: %2;"
        "  border: 1px solid %3;"
        "  border-bottom: none;"
        "  padding: 8px 16px;"
        "  margin-right: 2px;"
        "  border-top-left-radius: 4px;"
        "  border-top-right-radius: 4px;"
        "}"
        "QTabBar::tab:selected {"
        "  background-color: %3;"
        "}"
        "QTabBar::tab:hover {"
        "  background-color: %3;"
        "}"
        "QStatusBar {"
        "  background-color: %1;"
        "  color: %2;"
        "  border-top: 1px solid %3;"
        "}"
        "QLabel {"
        "  color: %2;"
        "}"
    ).arg(bgColor.name())
     .arg(textColor.name())
     .arg(borderColor.name());
    
    setStyleSheet(styleSheet);
    
    setWindowOpacity(settings.opacity() / 100.0);
}

void MainWindow::addNewTerminal(TerminalSession::ShellType shell) {
    TerminalWidget* terminal = new TerminalWidget(shell, this);
    
    QString tabTitle = shell == TerminalSession::PowerShell ? 
                       "PowerShell" : "Command Prompt";
    
    int index = m_tabWidget->addTab(terminal, tabTitle);
    m_tabWidget->setCurrentIndex(index);
    
    connect(terminal, &TerminalWidget::directoryChanged,
            this, &MainWindow::updateStatusBar);
    
    terminal->focusInput();
    updateStatusBar();
}

void MainWindow::onNewTab() {
    addNewTerminal(static_cast<TerminalSession::ShellType>(
        Settings::instance().defaultShell()));
}

void MainWindow::onCloseTab(int index) {
    if (m_tabWidget->count() == 1) {
        close();
        return;
    }
    
    QWidget* widget = m_tabWidget->widget(index);
    m_tabWidget->removeTab(index);
    widget->deleteLater();
}

void MainWindow::onTabChanged(int index) {
    Q_UNUSED(index);
    updateStatusBar();
}

void MainWindow::onSettings() {
    SettingsDialog dialog(this);
    connect(&dialog, &SettingsDialog::settingsChanged,
            this, &MainWindow::onSettingsChanged);
    dialog.exec();
}

void MainWindow::onSettingsChanged() {
    applyTheme();
    
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        TerminalWidget* terminal = qobject_cast<TerminalWidget*>(m_tabWidget->widget(i));
        if (terminal) {
            terminal->update();
        }
    }
}

void MainWindow::updateStatusBar() {
    TerminalWidget* current = qobject_cast<TerminalWidget*>(m_tabWidget->currentWidget());
    if (current) {
        QString dir = current->currentDirectory();
        m_directoryLabel->setText(QString(" 📁 %1 ").arg(dir));
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    bool hasRunningProcesses = false;
    
    for (int i = 0; i < m_tabWidget->count(); ++i) {
        TerminalWidget* terminal = qobject_cast<TerminalWidget*>(m_tabWidget->widget(i));
        if (terminal && terminal->isSessionRunning()) {
            hasRunningProcesses = true;
            break;
        }
    }
    
    if (hasRunningProcesses) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Confirm Close",
            "There are running processes. Do you want to close all terminals?",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (reply == QMessageBox::No) {
            event->ignore();
            return;
        }
    }
    
    event->accept();
}
