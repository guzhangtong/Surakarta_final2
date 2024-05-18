#ifndef RULE_H
#define RULE_H
#include <QWidget>
#include "stone.h"
#include "QPoint"
#include "Board.h"
class Rule
{
public:
    Rule();
    bool canMovel(int moveid,int x,int y,int killid);
};

#endif // RULE_H
