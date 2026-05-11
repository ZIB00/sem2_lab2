#include <QApplication> //The QApplication class manages the GUI application's control flow and main settings
#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
