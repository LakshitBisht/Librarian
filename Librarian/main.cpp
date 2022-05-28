#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
//starting the gui
    QApplication a(argc, argv);
//creating the main window object
    MainWindow w;
    w.show();
    return a.exec();
}
