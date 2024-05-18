
#include <QWidget>
#include "Board.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include "mainwindow.h"
#include "singlegame.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    /*QPixmap pixmap(":/loading/loadphoto");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();*/
    Board board;
    board.show();
   //splash.finish(&board);
    return app.exec();
}
