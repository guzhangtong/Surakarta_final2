#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lcdNumber_2->display(QString::number(countdownSecs));
    ui->lcdNumber->display(QString::number(countdownSecs));
    ui->stackedWidget->setCurrentIndex(0);
    m_aigame = new AIGame(this);
    //this->setStyleSheet("QMainWindow {background-color:rgb(7, 9, 4)}");
    this->setStyleSheet("QMainWindow {background-image:url(:/photo/back2.png)}");
    // 设置窗口固定大小
    setFixedSize(700,500); // 设置窗口宽度为800像素，高度为600像素
    // 禁用窗口的大小调整功能
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint); // 禁止用户调整窗口大小
    ui->gridLayout_2->addWidget(m_aigame);
ui->pushButton->setStyleSheet("position: absolute; top: 100px; left: 100px; width: 100px; height: 50px;");
ui->pushButton_2->setStyleSheet("position:absolute; top: 100px; left: 100px; width: 100px; height: 50px;");
ui->pushButton_3->setStyleSheet("position: absolute; top: 100px; left:100px; width: 100px; height: 50px;");
ui->pushButton_4->setStyleSheet("position: absolute; top: 100px; left: 100px; width: 100px; height: 50px;");
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::chooseModel);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::chooseModel_2);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::chooseModel_3);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::chooseModel_4);
    connect(ui->loseBtn_1,&QPushButton::clicked,this,&MainWindow::slot_endGameBox);
    connect(ui->loseBtn_2,&QPushButton::clicked,this,&MainWindow::slot_endGameBox);
    connect(ui->undoBtn_1,&QPushButton::clicked,this,&MainWindow::undo);
    connect(ui->undoBtn_2,&QPushButton::clicked,this,&MainWindow::undo);
    connect(&gamePlayTimer,&QTimer::timeout,this,&MainWindow::gamePlayTimeout);
    connect(m_aigame,&Board::sig_userChanged,this,&MainWindow::userChanged);
    connect(m_aigame,&Board::sig_finish,[=](bool user1){
        QString msg;
        if(user1){
            msg = "用户1输了\n游戏结束，您想要做什么？";
        }else {
            msg = "用户2输了\n游戏结束，您想要做什么？";
        }
        endGameBox(msg);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::chooseModel()
{
    qDebug() << "enter human vs AI";
    this->m_aigame->ai_mode[0] = false;
    this->m_aigame->ai_mode[1] = false;
    //此处应该切换到不同界面，等stage3联网做完再实现
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::chooseModel_2() {
    qDebug() << "enter human vs human";
    this->m_aigame->ai_mode[0] = false;
    this->m_aigame->ai_mode[1] = true;
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::chooseModel_3() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::chooseModel_4() {
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::slot_endGameBox()
{
    QObject *obj = sender();
    QString msg;
    if(obj == ui->loseBtn_1){
        msg = "用户1输了\n游戏结束，您想要做什么？";
    }else {
        msg = "用户2输了\n游戏结束，您想要做什么？";
    }
    endGameBox(msg);
}

void MainWindow::undo()
{
    //悔棋
    m_aigame->undo();
}



//用户切换
void MainWindow::userChanged()
{
    if(!gamePlayTimer.isActive())
        gamePlayTimer.start(1*1000);

    if(m_aigame->getUser1()){
        user2Count = countdownSecs;
    }else {
        user1Count = countdownSecs;
    }

}

void MainWindow::reset()
{
    m_aigame->reset();
    ui->lcdNumber_2->display(QString::number(countdownSecs));
    ui->lcdNumber->display(QString::number(countdownSecs));
    user1Count = countdownSecs;
    user2Count = countdownSecs;
}

void MainWindow::gamePlayTimeout()
{
    if(m_aigame->getUser1()){
        user1Count -=1;
        ui->lcdNumber_2->display(QString::number(countdownSecs));
        ui->lcdNumber->display(user1Count);
    }else {
        user2Count -=1;
        ui->lcdNumber->display(QString::number(countdownSecs));
        ui->lcdNumber_2->display(user2Count);
    }

    if(user1Count <=0 ) {
        endGameBox("用户1输了\n游戏结束，您想要做什么？");
    }else if(user2Count <=  0) {
        endGameBox("用户2输了\n游戏结束，您想要做什么？");
    }
}


void MainWindow::endGameBox(QString msg)
{
    QObject* obj = sender();
    gamePlayTimer.stop();

    // 创建一个消息框
    QMessageBox endGameBox;
    endGameBox.setText(msg);

    // 创建自定义按钮
    QPushButton *restartButton = new QPushButton("重新开始");
    QPushButton *exitButton = new QPushButton("结束游戏");

    // 将自定义按钮添加到消息框
    endGameBox.addButton(restartButton, QMessageBox::ActionRole);
    endGameBox.addButton(exitButton, QMessageBox::RejectRole);

    // 由于我们添加了自定义按钮，所以不使用标准按钮
    endGameBox.setStandardButtons(QMessageBox::NoButton);

    // 连接按钮的clicked信号到槽函数
    connect(restartButton, &QPushButton::clicked, this, [this, &endGameBox]() {
        // 用户点击了“重新开始”按钮
        endGameBox.close(); // 关闭消息框
        reset();
    });

    connect(exitButton, &QPushButton::clicked, this, [this, &endGameBox]() {
        // 用户点击了“结束游戏”按钮
        endGameBox.close(); // 关闭消息框
        exit(0);
    });

    // 显示消息框
    endGameBox.exec();
}






