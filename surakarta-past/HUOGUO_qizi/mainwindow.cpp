#include "mainwindow.h"
#include <QTextEdit>
#include <windows.h>

mainwindow::mainwindow(QWidget *parent)
    : QMainWindow{parent}
{
    setWindowTitle("HUOGUO_catcat");
    QTextEdit *edit = new QTextEdit;
    edit->setText("HUOGUOMAOMAO come ! ! !");
    setCentralWidget(edit);
    resize(60, 45); // 修正了resize函数的参数
    Sleep(1500);
}

