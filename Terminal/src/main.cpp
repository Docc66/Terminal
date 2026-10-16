#include <QApplication>
#include "MainWindow.h"
#include "Settings.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("NEXY Terminal");
    app.setOrganizationName("NEXY");
    app.setApplicationVersion("1.0.0");
    
    Settings::instance().load();
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
