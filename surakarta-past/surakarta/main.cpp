#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("cde");

    MainWindow w;
    w.show();

    return a.exec();
}
