#ifndef MYWIDGET_H
#define MYWIDGET_H
#include "GameModel.h"
#include <QWidget>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent =0);
    GameModel *game;
    GameType game_type;
    int clickPosRow,clickPosCol;
    bool selectPos=false;
    //绘制
    void paintEvent(QPaintEvent *event);
    void initGame();
    void initAIGame();
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void chessOneByPerson();
signals:
};

#endif // MYWIDGET_H
