#include "rule.h"
#include "Board.h"
#include <iostream>
#include "stone.h"
Rule::Rule() {

}

bool Rule::canMovel(int moveid,int x,int y,int killid)
{
    if (x >=5||y >= 5) {
        return false;  // 返回超出棋盘范围的非法移动原因
    }
    Board::myx=_s[moveid]._x;
    int d=-x;
    return true;
}
