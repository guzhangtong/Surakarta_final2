#include "singlegame.h"

singleGame::singleGame() {}
void singleGame::mouseReleaseEvent(QMouseEvent *ev){
    if(!this->bTurn)
    {
        return;
    }
    Board::mouseReleaseEvent(ev);
    if(!this->bTurn)
    {
        AIMove();
    }
}
int singleGame::findID(int x,int y)
{
    for(unsigned int j=0;j<24;j++)
    {
        if(_s[j]._dead)
            continue;
        if(_s[j]._x==x&&_s[j]._y==y)
            return j;
    }
    return -1;
}
void singleGame::AIMove()
{
    for(int i=0;i<12;i++)
    {
        for(int to_x=0;to_x<6;to_x++)
        {
            for(int to_y=0;to_y<6;to_y++)
            {
                int killid=this->findID(to_x,to_y);
                if(_s[killid]._color==_s[i]._color)
                    continue;
                if(canMovel(i,to_x,to_y,killid)&&killid!=0)
                {

                }

            }
        }
    }

}
