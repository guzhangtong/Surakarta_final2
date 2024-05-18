#ifndef BOARD_H
#define BOARD_H
#include <QWidget>
#include "stone.h"
#include "QPoint"
#include <QVector>
#include <QMainWindow>

class Board:public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent=0);
    Stone _s[24];
    int _select=-1;
    int clickPosRow,clickPosCol;
    bool selectPos=false;
    bool bTurn;
    QPoint center(int row,int col);//返回棋子坐标
     QPoint center(int id);//返回棋子坐标
    bool getRowCol(QPoint pt,int& x,int& y);
    void drawStone( QPainter& painter,int id);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool canMovel(int moveid,int x,int y,int killid);
   // bool GAMEOVER();

signals:
public slots:
};

const int BOARD_GRAD_SIZE=6;//棋盘大小
const int bianyuan=100;
const int CHESS_SIZE=15;//棋子大小
const int MARK_SIZE=10;
const int BLOCK_SIZE=40;//格子大小
const int POS_OFFSET=BLOCK_SIZE*0.6;//模糊度
const int THINK_TIME=700;

#endif // BOARD_H
