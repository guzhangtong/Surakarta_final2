#include "AIGame.h"
#include <QPainter>
#include <QTimer>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>

AIGame::AIGame(QWidget *parent){
    _level = 4;
}

// void AIGame::click(int id,int  row,int col)
// {
//     if(bTurn==1)
//     {        /* 启动0.1秒定时器，在0.1秒后电脑再思考 */
//         AIgo();
//         return;
//     }
//     Board::click(id, row, col);
// }

void AIGame::mouseReleaseEvent(QMouseEvent *ev) {
    copyStone(_s_back, _s);
    if (ai_mode[bTurn])
    {
        copyStone(_s_back, _s);
        // qDebug() << "ai agent operation";
        AIgo();
        int dead1=0,dead2=0;
        for(int i=0;i<24;i++)
        {
            if(_s[i]._dead==true&&i<12)
            {
                dead1++;
            }
            if(_s[i]._dead==true&&i>=12)
                dead2++;
        }
        if(dead1==12)
        {
            emit sig_finish(true);
            return;
        }

        if(dead2==12)
        {
            emit sig_finish(false);
            return;
        }
        return;
    }
    copyStone(_s_back, _s);
    Board::click(ev->pos());
}

void AIGame::AIgo()
{
    //qDebug() << "enter Aigo()";
    Step* step = getBestMove();
    qDebug() << "move info: " << step->_moveid << " " << step->_killid << " " << step->_rowTo << " " << step->_colTo;
    moveStone(step->_moveid, step->_killid,
              step->_rowTo, step->_colTo);
    Board::step=step->_moveid;
    delete step;

    // drawing logic

    update();
}

void AIGame::getAllMove(QVector<Step *> &steps)
{
    for(int i=0;i<12;i++)
    {
        if(_s[i]._dead==true)
        {
            continue;
        }
        for(int row=0;row<6;row++)
        {
            for(int col=0;col<6;col++)
            {
                int toid=-1;
                for( int m=0;m<24;m++)
                {
                    if(_s[m]._x==row&&_s[m]._y==col&&_s[m]._dead==false)
                    {
                        toid=m;
                        break;
                    }
                }//判断是否走空白
                if(toid<12&&toid!=-1)
                {
                    continue;
                }
                 if(canMovel(i,row,col,toid)==true)
                {
                    saveStep(i,toid,row,col,steps);
                }
            }
        }
    }
}
void AIGame::fakeMove(Step* step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void AIGame::unfakeMove(Step* step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

/* 评价局面分 */
int AIGame::calcscore()
{
    int TotalScore_1 = 0;
    int TotalScore_2 = 0;
    // 黑棋分的总数 - 红旗分的总数
    for(int i=0; i<12; ++i)
    {
        if(_s[i]._dead) continue;
        TotalScore_1 += 10;
    }
    for(int i=12; i<24; ++i)
    {
        if(_s[i]._dead) continue;
        TotalScore_2 +=10;
    }

    return TotalScore_1 - TotalScore_2;
}

int AIGame::getMaxScore(int level, int curMinScore)
{
    if(level == 0) return calcscore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllMove(steps);   // 是红旗的possiblemove

    int maxScore = -100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMinScore(level-1, maxScore);
        unfakeMove(step);
        delete step;

        if(score >= curMinScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score > maxScore)
        {
            maxScore = score;
        }

    }
    return maxScore;
}

int AIGame::getMinScore(int level, int curMaxScore)
{


    if(level == 0) return calcscore();

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllMove(steps);   // 是红旗的possiblemove

    int minScore = 100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMaxScore(level-1, minScore);
        unfakeMove(step);
        delete step;

        if(score <= curMaxScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }

        if(score < minScore)
        {
            minScore = score;
        }

    }
    return minScore;
}

Step* AIGame::getBestMove()
{
    /*
        2.试着走一下
        3.评估走的结果
        4.取最好的结果作为参考
    */

    // 1.看看有那些步骤可以走
    QVector<Step*> steps;
    getAllMove(steps);

    // 2.试着走一下
    // 3.评估走的结果
    int maxScore = -100000;
    Step* ret = NULL;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();

        fakeMove(step);
        int score = getMinScore(_level-1, maxScore);
        unfakeMove(step);

        if(score > maxScore)
        {
            maxScore = score;
            if(ret) delete ret;
            ret = step;
        }
        else
        {
            delete step;
        }
    }

    // 4.取最好的结果作为参考
    return ret;
}

