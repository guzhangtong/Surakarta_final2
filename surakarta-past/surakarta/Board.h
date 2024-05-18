#ifndef BOARD_H
#define BOARD_H
#include <QWidget>
#include "stone.h"
#include "QPoint"
#include <QVector>
#include <QMainWindow>
#include <qpushbutton.h>
#include "step.h"

class Board:public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent=0);
    QVector<Stone> _s;
    QVector<Stone> _s_back;
    QVector<Stone> _s_reset;
    QVector<Step*> _steps;
    int _select=-1;
    int step=-1;
    int clickPosRow,clickPosCol;
    bool selectPos=false;
    bool bTurn;
    bool nearDeath_1=false;
     bool nearDeath_2=false;

    // bool ai_mode[2] = {false, false};

    int can_go_x[40],can_go_y[40];
    QPoint center(int row,int col);//返回棋子坐标
    QPoint center(int id);//返回棋子坐标
    void moveStone(int moveid, int killid, int row, int col);
    void moveStone(int moveid, int row, int col);
    bool getRowCol(QPoint pt,int& x,int& y);
    void drawStone( QPainter& painter,int id);
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool canMovel(int moveid,int x,int y,int killid);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);
    void reliveStone(int id);
    void killStone(int id);
    void undo();
    bool getUser1(){return bTurn;}
    void reset();
    void click(int id,int  row,int col);
    void click(QPoint pt);
signals:
    void sig_userChanged();
    void sig_finish(bool user1);
protected:
    void copyStone(QVector<Stone>&s1,QVector<Stone>&s2);
    void init();

};

const int BOARD_GRAD_SIZE=6;//棋盘大小
const int bianyuan=100;
const int CHESS_SIZE=15;//棋子大小
const int MARK_SIZE=10;
const int BLOCK_SIZE=40;//格子大小
const int POS_OFFSET=BLOCK_SIZE*0.6;//模糊度
const int THINK_TIME=700;

#endif // BOARD_H
