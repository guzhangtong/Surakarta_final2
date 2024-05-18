#include "serverwindow.h"
#include "./ui_serverwindow.h"

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);//本地IP，所有电脑都可以用这个IP指向自己

    //初始化参数?
    //QTcpSocket * client; // 作为服务端时，已连接机器的IP
    //client=nullptr;
    //列表邀请的内容？



    //创建一个服务端   创建一个服务器对象，其父对象为当前窗口
    server = new NetworkServer(this);

    ui->ShowClient1->setReadOnly(true);
    ui->ShowClient2->setReadOnly(true);

    //服务端开启监听
    connect(ui->PortButton, &QPushButton::clicked, this, &ServerWindow::listen_port);
    //重启服务端
    connect(ui->restart, &QPushButton::clicked, this, &ServerWindow::restart_server);
    //receive是服务端收到消息后发出的信号，receive_from_client 是处理这个信号的槽函数
    connect(server, &NetworkServer::receive, this, &ServerWindow::receive_from_client);

}

//服务端 监听函数
void ServerWindow::listen_port() {
    this->port = ui->PortEdit->text().toInt();
    server->listen(QHostAddress::Any, this->port);
    //一行代码搞定监听，它会在所有ip地址上监听指定端口port
    //所谓监听，就是在网络上服务器要开始盯住某个端口，此时客户端可以通过这个端口和服务器取得联系
    //QHostAddress::Any 表示监听所有 ip 地址，在不考虑安全的情况下，这是一个比较方便的选择
    ui->PortButton->setEnabled(false);
    ui->PortEdit->setReadOnly(true);
    ui->PortEdit->setText("Listening...");
}

//服务端 删除客户端函数  在客户端发送LEAVE_OP后将其删除
void ServerWindow::remove_client(QTcpSocket* client) {
    if (client == client1) {
        client1 = nullptr;
        ui->ShowClient1->setText("");
    }
    else if (client == client2) {
        client2 = nullptr;
        ui->ShowClient2->setText("");
    }
    clients.remove(client);
}

//服务端 信息处理函数   处理客户端传来的OP和数据 LEAVE_OP  CHAT_OP
//首先处理客户端的连接和断开，然后处理从客户端收到的消息，显示在界面上并转发给另一个客户端
//你们在游戏里要做类似的事情，只不过处理不仅仅是显示，而是对应的游戏逻辑
void ServerWindow::receive_from_client(QTcpSocket* client, NetworkData data) {

    //LEAVE_OP
    if (data.op == OPCODE::LEAVE_OP) {
        remove_client(client);
        return;
    }

    if (!clients.contains(client)) {
        if (clients.size() >= max_clients) {
            QMessageBox::warning(this, "Warning", "The server is full!");
            return;
        }
        clients.insert(client);
        if (!client1)
            client1 = client;
        else if (!client2)
            client2 = client;
    }

    //CHAT_OP   判断是哪个客户端写的  转给另一个人
    if (client == client1) {//client1 连接上了
        this->ui->ShowClient1->setText(data.data2);
        if (client2 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client2, data);
    }
    else if (client == client2) {
        this->ui->ShowClient2->setText(data.data2);
        if (client1 && data.op == OPCODE::CHAT_OP)
            send_to_another_client(client1, data);
    }
    else
        QMessageBox::warning(this, "Warning", "Unknown client!");


    switch(data.op)
    {
    case OPCODE::READY_OP:
        if(!onGame)
        {
            ui->StatusLabel->setText("new request");
            HisName=data.data1;
            GameRequest(data.data1,data.data2);
        }
        break;

    case OPCODE::MOVE_OP:
        CodetoNumber(data.data1,data.data2);
        break;

        //RESIGN_OP  认输
    case OPCODE::GIVEUP_OP:
        this->server->send(client,NetworkData(OPCODE::GIVEUP_END_OP,MyName,NULL));
        break;

    }



}

//向另一个客户端发送信息,这个程序里实际上做的事情是转发消息
void ServerWindow::send_to_another_client(QTcpSocket* another, NetworkData data) {
    this->server->send(another, data);
}

//重启服务端函数  像其他 QObject 一样，你需要 disconnect 再 connect 回来一些信号槽
void ServerWindow::restart_server() {
    server->close();
    clients.clear();
    client1 = nullptr;
    client2 = nullptr;
    disconnect(server, &NetworkServer::receive, this, &ServerWindow::receive_from_client);
    delete server;
    server = new NetworkServer(this);//创建一个新服务端
    ui->PortButton->setEnabled(true);//开启监听
    ui->PortEdit->setReadOnly(false);
    ui->PortEdit->setText(QString::number(this->port));
    ui->ShowClient1->setText("");
    ui->ShowClient2->setText("");
    connect(server, &NetworkServer::receive, this, &ServerWindow::receive_from_client);
}

ServerWindow::~ServerWindow() {
    delete ui;
    delete server;
}
