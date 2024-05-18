#include "Board.h"
#include "AIGame.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QLabel>
#define GetRowCol(__row, __col, __id) \
int __row = _s[__id]._x;\
    int __col = _s[__id]._y

Board::Board(QWidget *parent):QWidget(parent)
{

    init();
}
void Board::paintEvent(QPaintEvent *)
{   //绘制棋盘
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setPen(QPen(Qt::white, 2));
    for (int i = 0; i < BOARD_GRAD_SIZE; i++) {
        painter.drawLine(bianyuan + BLOCK_SIZE * i, bianyuan,
                         bianyuan + BLOCK_SIZE * i,bianyuan+(BOARD_GRAD_SIZE-1)*BLOCK_SIZE);
        painter.drawLine(bianyuan, BLOCK_SIZE * i + bianyuan,
                         bianyuan+(BOARD_GRAD_SIZE-1) *BLOCK_SIZE, bianyuan + BLOCK_SIZE * i);
    }
    int x =bianyuan; // 圆心的x坐标
    int y = bianyuan; // 圆心的y坐标
    int radius =BLOCK_SIZE; // 圆的半径
    int startAngle = 0; // 起始角度
    int spanAngle = 270 * 16; // 跨度角度，这里是3/4圆的角度（以1/16度为单位）
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    radius =BLOCK_SIZE*2;
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    x =bianyuan + BLOCK_SIZE * 5; // 圆心的x坐标
    y = bianyuan; // 圆心的y坐标
    radius =BLOCK_SIZE; // 圆的半径
    startAngle = -90*16; // 起始角度
    spanAngle =270 * 16; // 跨度角度，这里是3/4圆的角度（以1/16度为单位）
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    radius =BLOCK_SIZE*2;
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    x =bianyuan + BLOCK_SIZE * 5; // 圆心的x坐标
    y =bianyuan + BLOCK_SIZE * 5; // 圆心的y坐标
    radius =BLOCK_SIZE; // 圆的半径
    startAngle = -180*16; // 起始角度
    spanAngle = 270 * 16; // 跨度角度，这里是3/4圆的角度（以1/16度为单位）
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    radius =BLOCK_SIZE*2;
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    x =bianyuan; // 圆心的x坐标
    y =bianyuan + BLOCK_SIZE * 5; // 圆心的y坐标
    radius =BLOCK_SIZE; // 圆的半径
    startAngle = 90*16; // 起始角度
    spanAngle = 270 * 16; // 跨度角度，这里是3/4圆的角度（以1/16度为单位）
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    radius =BLOCK_SIZE*2;
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>=0&&clickPosRow<BOARD_GRAD_SIZE&&clickPosCol>=0&&clickPosCol<BOARD_GRAD_SIZE)
    {

        brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(bianyuan+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,bianyuan+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,CHESS_SIZE*0.5,CHESS_SIZE*0.5);
    }//绘制棋子
    for(int i=0;i<24;i++){
        drawStone(painter,i) ;
    }

    // 设置点的坐标
    if(step!=-1){
    int pointX =bianyuan + BLOCK_SIZE * _s[step]._x;
    int pointY =bianyuan + BLOCK_SIZE *  _s[step]._y;
    int pointSize = 5;
    painter.setPen(QPen(Qt::white, pointSize));
    painter.drawPoint(pointX, pointY); // 绘制点
    }
}
QPoint Board::center(int x, int y)
{
    QPoint ret;
    ret.setX(bianyuan + BLOCK_SIZE * x);
    ret.setY(bianyuan + BLOCK_SIZE * y);
    return ret;
}
QPoint Board::center(int id)
{
    return center(_s[id]._x,_s[id]._y);
}
void Board::drawStone(QPainter& painter,int id)//画棋子
{

    // 使用RGB值创建自定义颜色
    if(_s[id]._dead)
        return;
    if(_s[id]._color){
        QColor customColor(240,212,137);
        QBrush customBrush(customColor);// 将自定义颜色的画刷应用到绘图工具上
        painter.setBrush(customBrush);}// 使用RGB值(255, 192, 203)创建自定义颜色
    else{
        QColor customColor(228,92,44);
        QBrush customBrush(customColor);// 将自定义颜色的画刷应用到绘图工具上
        painter.setBrush(customBrush);}
    if(id==_select)
    {
        if(id==_select)
        {
            for(int i=0;i<=35;i++)
            {
                // 设置发光效果的画笔和画刷
                painter.setBrush(QColor(250, 244, 240)); // 设置半透明的白色画刷
                // 绘制一个发光的十字箭头
                int arrowSize = 7; // 箭头大小
                // 绘制横线
                painter.drawRect(bianyuan + BLOCK_SIZE * can_go_x[i] - arrowSize,bianyuan + BLOCK_SIZE *  can_go_y[i] - 2, 2 * arrowSize, 4);
                // 绘制竖线
                painter.drawRect(bianyuan + BLOCK_SIZE * can_go_x[i] - 2,bianyuan + BLOCK_SIZE *  can_go_y[i] - arrowSize, 4, 2 * arrowSize);
            }
        }
        QColor customColor(255,255,255);
        QBrush customBrush(customColor);// 将自定义颜色的画刷应用到绘图工具上
        painter.setBrush(customBrush);}
    painter.drawEllipse(center(id),CHESS_SIZE,CHESS_SIZE);

}
bool Board::getRowCol(QPoint  pt,int &x,int &y)//判断是否选择成功
{
    for(x=0;x<=5;x++)
    {
        for(y=0;y<=5;y++)
        {
            QPoint c=center(x,y);
            int dx=c.x()-pt.x();
            int dy=c.y()-pt.y();
            int dist=dx*dx+dy*dy;
            if(dist<CHESS_SIZE*CHESS_SIZE)
                return true;
        }
    }
    return false;
}
void Board::reliveStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = false;
}
void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;
}
void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._x = row;
    _s[moveid]._y = col;
    bTurn = !bTurn;
}
void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}
void Board::moveStone(int moveid, int killid, int row, int col)
{
    saveStep(moveid, killid, row, col, _steps);

    killStone(killid);
    moveStone(moveid, row, col);
}

void Board::click(QPoint pt)
{

    //将pt转换为坐标
    int x,y;
    bool bRet=getRowCol(pt,x,y);
    if(bRet==false)
        return;
    int i=0;
    int clickid=-1;
    int dead1=0,dead2=0;
    for( i=0;i<24;i++)
    {
        if(_s[i]._x==x&&_s[i]._y==y&&_s[i]._dead==false)
        {
            clickid=i;
            break;
        }
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
    click(clickid,x,y);
}
void Board::click(int clickid, int x, int y){
    if(clickid!=-1&&bTurn==_s[clickid]._color)
    {

        int flag=0;
        for(int i=0;i<=35;i++)//CAN GO TO
        {
            can_go_x[i]=1000;
            can_go_y[i]=1000;

        }
        for(int i=0;i<=5;i++)//CAN GO TO
        {
            for(int j=0;j<5;j++)
            {
                int toid=-1;
                for( int m=0;m<24;m++)
                {
                    if(_s[m]._x==i&&_s[m]._y==j&&_s[m]._dead==false)
                    {
                        toid=m;
                        break;
                    }
                }
                if(canMovel(clickid,i,j,toid)==true)
                {
                    can_go_x[flag]=i;
                    can_go_y[flag]=j;
                    flag++;
                }
            }
        }
        _select=clickid;
        update();

    }
    else if( _select!=-1)
    {
        if(canMovel(_select,x,y,clickid)==true)
        {
            /*判断能否走棋子*/
            _s[_select]._x=x;
            _s[_select]._y=y;
            if(clickid!=-1)
            {
                _s[clickid]._dead=true;
            }
            step=_select;
            _select=-1;
            bTurn=!bTurn;
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

            emit sig_userChanged();

            update();

        }
    }
}
void Board::mouseReleaseEvent(QMouseEvent *ev)//选择棋子
{
    copyStone(_s_back,_s);
    click(ev->pos());
}
//save
//规则
int weizhi_line_1(unsigned int i, unsigned int j)
{
    if (i == 1)
        return j;
    if (j == 4)
        return i + 6;
    if (i == 4)
        return 17 - j;
    if(j==1)
        return 23 - i;
    return 250;
}  // 在第一条吃子链条上时，根据坐标给该位置标序号
int weizhi_line_2(unsigned int i, unsigned int j)
{
    if (i == 2)
        return j;
    if (j == 3)
        return i + 6;
    if (i == 3)
        return 17 - j;
    if(j==2)
        return 23 - i;
    return 250;
}  // 在第二条吃子链条上时，根据坐标给该位置标序号
int which_line(unsigned int i, unsigned int j) {
    int f1 = 0, f2 = 0;
    if ((i == 1) || (i == 4) || (j == 1) || (j == 4))
        f1 = 1;
    if ((i == 2) || (i == 3) || (j == 2) || (j == 3))
        f2 = 2;
    return f1 + f2;
}  // 判断在第几条吃子链条上/可以是1/2/3（3代表两个都有）
int PO2_1(int po_to) {
    if (po_to == 4 || po_to == 10 || po_to == 16)
        return po_to + 3;
    if (po_to == 1)
        return 22;
    return 1000;
}
bool Board::canMovel(int moveid,int x,int y,int killid)
{
    int from_x=_s[moveid]._x,from_y=_s[moveid]._y,to_x=x,to_y=y;
    if (from_x >=6 || from_y >= 6 || to_x>= 6 || to_y >=6) {
        return false;  // 返回超出棋盘范围的非法移动原因
    }
    if (killid==-1) {
        int xf, yf;
        if (from_x <= to_x)
            xf = to_x- from_x;
        else
            xf = from_x -to_x;
        if (from_y <= to_y)
            yf = to_y - from_y;
        else
            yf = from_y - to_y;
        if (xf <= 1 && yf <= 1)                                         //&& (to_x- from_x) >= -1 && (to_y - from_y) <= 1 && (to_y - from_y) >= -1)
            return true;  // 判断是否是在周围一格内
        else
            return false;  // 不是在周围一格内
    }
    if(_s[moveid]._color==_s[killid]._color)
    {
        _select=killid;
        update();
        return false;
    }
    if ( ((to_x== 0) + (to_y == 0) + (to_x== 5) + (to_y ==5) == 2) || ((from_x == 0) + (from_y == 0) + (from_x ==5) + (from_y ==5) == 2)){
        return false;  // 特判断 在四个角的位置是吃不到的
    }

    int po_from, po_to, line_from, line_to;
    line_from = which_line(from_x, from_y);
    line_to = which_line(to_x, to_y);  // 判断from棋子和to棋子在which——line上
    if (line_from != 3 && line_from != line_to && line_to != 3) {
        return false;  // 如果两个棋子不在同一条吃子路径上，肯定没救了
    }
    int line_1[1000] = {0}, line_2[1000] = {0};  // 数组存放改路径上有or无子 柚子==1，无子==0
    if (line_from == 1 || line_to == 1 || (line_to == 3 && line_from == 3)) {
        for(unsigned int j=0;j<24;j++)
        {
            if(_s[j]._dead)
                continue;
            line_1[weizhi_line_1(_s[j]._x,_s[j]._y)]=1;
            line_1[PO2_1(weizhi_line_1(_s[j]._x,_s[j]._y))]=1;
        }
        po_from = weizhi_line_1(from_x, from_y);
        po_to = weizhi_line_1(to_x, to_y);
        int po_to2 = PO2_1(po_to), po_from2 = PO2_1(po_from);  // 再交差点时候，可能会有两个序号，但是实际上只存了一格，所以需要存存po2. // 对于要去的地方其实也有交叉点呢，去到任何一个序号都算完成任务
        line_1[po_from2] = 0;
        line_1[po_from] = 0;  // 由于是可以绕过自己的，所以....对于自己来说是无子的啦！
        if ((po_from == 0 && po_to == 23) || (po_from == 23 && po_to == 0) || (po_from == 17 && po_to == 18) || (po_from == 11 && po_to == 12) || (po_from == 12 && po_to == 11) || (po_from == 18 && po_to == 17) || (po_from == 5 && po_to == 6) || (po_from == 6 && po_to == 5))
            return true;  // 特判，直接在旋吃轨道两旁的当然可以吃。
        int IS_xuanchi = 0;
        for (int i = po_from + 1;; i++) {  // 向着序号增大的方向吃
            if (i == 24)                   // 这是一个首位相连的链。
                i = 0;
            if ((i == po_to || i == po_to2) && IS_xuanchi == 1)  // 只要到达任意一个序号都可以+通过旋吃弧线
                return true;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)  // 只要到达这几个地方，就可以认为这是通过旋吃弧线的。
                IS_xuanchi = 1;
            if (line_1[i] != 0)  // 碰到阻碍的话就无法前进了。
                break;
        }
        IS_xuanchi = 0;                    // 旋吃归零
        for (int i = po_from - 1;; i--) {  // 向着序号减小的方向吃
            if (i == -1)
                i = 23;
            if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                return true;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                IS_xuanchi = 1;
            if (line_1[i] != 0)
                break;
        }
        if (po_from == 1 || po_from == 4 || po_from == 10 || po_from == 16) {  // 同上
            IS_xuanchi = 0;
            for (int i = po_from2 + 1;; i++) {
                if (i == 24)
                    i = 0;
                if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                    return true;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    IS_xuanchi = 1;
                if (line_1[i] != 0)
                    break;
            }
            IS_xuanchi = 0;
            for (int i = po_from2 - 1;; i--) {
                if (i == -1)
                    i = 23;

                if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                    return true;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    IS_xuanchi = 1;
                if (line_1[i] != 0)
                    break;
            }
        }
        if (line_from == 1 || line_to == 1)
            return false;
    }
    if (line_from == 2 || line_to == 2 || (line_to == 3 && line_from == 3)) {  // 另外一条路径上，与前面基本相同，所以没有特意标注释
        for(unsigned int j=0;j<24;j++)
        {
            if(_s[j]._dead)
                continue;
            line_2[weizhi_line_2(_s[j]._x,_s[j]._y)]=1;
            if (weizhi_line_2(_s[j]._x,_s[j]._y) == 3 || weizhi_line_2(_s[j]._x,_s[j]._y) == 9 || weizhi_line_2(_s[j]._x,_s[j]._y) == 15)
                line_2[weizhi_line_2(_s[j]._x,_s[j]._y)+5]=1;
            if (po_from == 2)
                line_2[21]=1;
        }
        po_from = weizhi_line_2(from_x, from_y);
        po_to = weizhi_line_2(to_x, to_y);  //???z
        int po_to2 = 100, po_from2 = 100;
        if (po_from == 3 || po_from == 9 || po_from == 15)
            po_from2 = po_from + 5;
        if (po_from == 2)
            po_from2 = 21;
        line_2[po_from2] = 0;
        line_2[po_from] = 0;
        if (po_to == 3 || po_to == 9 || po_to == 15 || po_to == 2)  // 3=8 9=14 15=20 21=2
        {
            if (po_to == 3 || po_to == 9 || po_to == 15) {
                po_to2 = po_to + 5;
            }
            if (po_to == 2)
                po_to2 = 21;
        }
        if ((po_from == 0 && po_to == 23) || (po_from == 23 && po_to == 0) || (po_from == 17 && po_to == 18) || (po_from == 11 && po_to == 12) || (po_from == 12 && po_to == 11) || (po_from == 18 && po_to == 17) || (po_from == 5 && po_to == 6) || (po_from == 6 && po_to == 5))
            return true;
        int IS_xuanchi = 0;
        for (int i = po_from + 1;; i++) {
            if (i == 24)
                i = 0;

            if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                return true;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                IS_xuanchi = 1;
            if (line_2[i] != 0)
                break;
        }
        IS_xuanchi = 0;
        for (int i = po_from - 1;; i--) {
            if (i == -1)
                i = 23;

            if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                return true;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                IS_xuanchi = 1;
            if (line_2[i] != 0)
                break;
        }
        if (po_from == 3 || po_from == 9 || po_from == 15 || po_from == 2)  // 3=8 9=14 15=20 21=2
        {
            if (po_from == 3 || po_from == 9 || po_from == 15) {
                po_from = po_from + 5;
            }
            if (po_from == 2)
                po_from = 21;
            IS_xuanchi = 0;
            for (int i = po_from + 1;; i++) {
                if (i == 24)
                    i = 0;

                if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                    return true;
                if (line_2[i] != 0)
                    break;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    IS_xuanchi = 1;
            }
            IS_xuanchi = 0;
            for (int i = po_from - 1;; i--) {
                if (i == -1)
                    i = 23;

                if ((i == po_to || i == po_to2) && IS_xuanchi == 1)
                    return true;
                if (line_2[i] != 0)
                    break;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    IS_xuanchi = 1;
            }
        }

        return false;
    }
    return false;

}

void Board::undo()
{
    if(_s == _s_back) {
        return ;
    }
    copyStone(_s,_s_back);
    bTurn=!bTurn;
    emit sig_userChanged();
    update();
}

void Board::reset()
{
    init();
    update();
}

void Board::copyStone(QVector<Stone> &s1, QVector<Stone> &s2)
{
    s1 = s2;
}

void Board::init()
{
    _s.clear();
    _s_back.clear();
    _s_reset.clear();
    for(int i=0;i<24;i++)
    {
        Stone s;
        s.init(i);
        _s.push_back(s);
    }

    copyStone(_s_back,_s);
    copyStone(_s_reset,_s);
    _select=-1;
    bTurn=true;
}

