#include "mainwindow.h"

#include <QApplication>
#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>
#include <firebase.h>


//https://stackoverflow.com/questions/3211272/qt-hide-minimize-maximize-and-close-buttons


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    mainWindow.showMaximized();

    QGuiApplication::setQuitOnLastWindowClosed( false);

    return a.exec();
}
