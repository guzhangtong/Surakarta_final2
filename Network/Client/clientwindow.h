#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include "../../NetworkLibrary/networkdata.h"
#include "../../NetworkLibrary/networksocket.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ClientWindow;
}
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private:
    int port = 10086;
    QString ip = "127.0.0.1";
    NetworkSocket *socket;
    Ui::ClientWindow *ui;

    QString MyName;
    QString HisName;
    //我方持棋颜色
    int MyColor;
    //是否在对局
    bool onGame;
    //是否已发送认输信号
    bool ResignSent;
    bool TimeoutSent;

private slots:
    void connected_successfully();
    void connectToServer();
    void disconnectFromServer();

    //发送聊天信息
    void sendMessage();

    //发送对局请求
    void sendRequest();

    //发送落子位置
    void Move(int ,int );

    //认输RESIGH
    void Resign();

    //超时结束
    void Timeout();

    //离开LEAVE
    void Leave();

    //客户端信息处理函数
    void receiveMessage(NetworkData);

    //重设姓名、IP和端口
    void reset();

    //重设姓名
    void resetName();

    //重启客户端 客户端要求再来一局
    void reconnect();


};
#endif // CLIENTWINDOW_H
