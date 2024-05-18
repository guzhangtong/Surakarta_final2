#ifndef SINGLEGAME_H
#define SINGLEGAME_H
#include "Board.h"
class singleGame :public Board
{
public:
    singleGame();
    void mouseReleaseEvent(QMouseEvent *ev);
    void AIMove();
    void getAllpossibleMove();
    int findID(int x,int y);
};

#endif // SINGLEGAME_H
