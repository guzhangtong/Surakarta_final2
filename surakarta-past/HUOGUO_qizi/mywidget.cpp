#include "mywidget.h"
#include <QPainter>
#include "GameModel.h"
MyWidget::MyWidget(QWidget *parent)
    : QWidget{parent}
{}
void MyWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    int x = bianyuan ; // 圆心的x坐标
    int y = bianyuan; // 圆心的y坐标
    int radius = 50; // 圆的半径
    int startAngle = 0; // 起始角度
    int spanAngle = 270 * 16; // 跨度角度，这里是3/4圆的角度（以1/16度为单位）
    painter.drawArc(x - radius, y - radius, radius * 2, radius * 2, startAngle, spanAngle); // 绘制3/4圆
    for (int i = 0; i < BOARD_GRAD_SIZE; i++) {
        painter.drawLine(bianyuan + BLOCK_SIZE * i, bianyuan,
                         bianyuan + BLOCK_SIZE * i,bianyuan+(BOARD_GRAD_SIZE-1)*BLOCK_SIZE);
        painter.drawLine(bianyuan, BLOCK_SIZE * i + bianyuan,
                         bianyuan+(BOARD_GRAD_SIZE-1) *BLOCK_SIZE, bianyuan + BLOCK_SIZE * i);
    }
    //绘制选中点
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(clickPosRow>=0&&clickPosRow<BOARD_GRAD_SIZE&&clickPosCol>=0&&clickPosCol<BOARD_GRAD_SIZE&&game->gameMapVec[clickPosRow][clickPosCol]==0)
    {
        if(game->playerFlag)
            brush.setColor(Qt::black);//棋子颜色参数
        else
            brush.setColor(Qt::white);
        painter.setBrush(brush);
        painter.drawRect(bianyuan+BLOCK_SIZE*clickPosCol-MARK_SIZE/2,bianyuan+BLOCK_SIZE*clickPosRow-MARK_SIZE/2,CHESS_SIZE*0.5,CHESS_SIZE*0.5);
    }

}
