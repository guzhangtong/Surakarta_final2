#ifndef GAMEMODEL_H
#define GAMEMODEL_H
#include "QPainter"
#include <vector>
enum GameType
{
    MAN,//人人对战
    AI//人机对战
};
enum GameStatus
{
    PLAYING,//未结束
    WIN,//赢了
    DEAD//输了
};
const int BOARD_GRAD_SIZE=6;//棋盘大小
const int bianyuan=100;
const int CHESS_SIZE=15;//棋子大小
const int MARK_SIZE=10;
const int BLOCK_SIZE=40;//格子大小
const int POS_OFFSET=BLOCK_SIZE*0.6;//模糊度
const int THINK_TIME=700;

class GameModel{
public:
    GameModel(){};
public:
    std::vector<std::vector<int>> gameMapVec;//空白为0，黑子1，白字-1
    std::vector<std::vector<int>> ScoreMvpVec;
    bool playerFlag;//标记下棋方
    GameType gameType;//游戏模式
    GameStatus gameStatus;//游戏状态
    void startGame(GameType type);
    void calculateScore();
    void actionByPerson(int row,int col);
    void actionByAI(int &clickRow,int &clickCol);
    void updateGameMap(int row,int col);
    bool isWin(int row,int col);//判断游戏是否胜利
    bool isDeadGame();//判断是否和其
};


#endif // GAMEMODEL_H
