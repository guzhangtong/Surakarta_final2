#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>
#include "../../NetworkLibrary/networkdata.h"
#include "../../NetworkLibrary/networkserver.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
class ServerWindow;
}
QT_END_NAMESPACE

class ServerWindow: public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private:
    int port = 10086;
    const int max_clients = 2;
    Ui::ServerWindow *ui;
    QSet<QTcpSocket*> clients;


    //服务端
    NetworkServer* server = nullptr;
    //作为服务端时，已连接机器的IP
    QTcpSocket* client1 = nullptr;
    QTcpSocket* client2 = nullptr;
    // 向另一个客户端发送信息,这个程序里实际上做的事情是转发消息    转发CHAT_OP的内容
    void send_to_another_client(QTcpSocket* another, NetworkData data);
    // 断开连接函数
    void remove_client(QTcpSocket* client);

    //READY处理函数
    void GameRequest(QString ,QString );
    //是否在对局
    bool onGame;

    //编码与棋盘坐标转换  即MOVE_OP中用到的坐标编码
    void CodetoNumber(QString code);

    QString MyName;
    QString HisName;

private slots:
    void listen_port();

    // 服务端信息处理函数
    void receive_from_client(QTcpSocket *client, NetworkData data);

    // 重启服务端函数  需要disconnect 再connect回来一些信号槽
    void restart_server();

    //列表邀请相关的信号槽

    //同意对局并开始
    void StartGame();

    //拒绝对局
    void RejectGame(QString );


    //signals:?
    //窗口被关闭时，唤出主窗口   ?

};

#endif // SERVERWINDOW_H
