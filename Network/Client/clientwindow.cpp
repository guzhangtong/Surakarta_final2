#include "clientwindow.h"
#include "./ui_clientwindow.h"


ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);//本地IP
    ui->ip_edit->setText(ip);
    ui->port_edit->setText(QString::number(port));
    ui->send_button->setEnabled(false);//发送按钮
    ui->disconnect_button->setEnabled(false);//断开连接按钮
    ui->receive_edit->setReadOnly(true);

    //创建一个客户端,其父对象为当前窗口
    socket = new NetworkSocket(new QTcpSocket(this), this);
    //connected 是客户端连接成功后发出的信号
    connect(socket->base(), &QTcpSocket::connected, this, &ClientWindow::connected_successfully);
    //客户端连接服务器
    connect(ui->connect_button, &QPushButton::clicked, this, &ClientWindow::connectToServer);
    //客户端断开连接
    connect(ui->disconnect_button, &QPushButton::clicked, this, &ClientWindow::disconnectFromServer);
    //客户端发送信息
    connect(ui->send_button, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    //receive是客户端收到消息后发出的信号，receiveMessage 是处理这个信号的槽函数
    connect(socket, &NetworkSocket::receive, this, &ClientWindow::receiveMessage);

    //客户端连接失败
    connect(socket->base(), &QAbstractSocket::disconnected, [=]() {
        QMessageBox::critical(this, tr("Connection lost"), tr("Connection to server has closed"));
    });

}


//客户端 等待连接函数
void ClientWindow::connectToServer() {
    this->ip = ui->ip_edit->text();//设置ip
    this->port = ui->port_edit->text().toInt();//设置端口
    socket->hello(ip, port);   // 连接服务器 ip:port
    this->socket->base()->waitForConnected(2000); // 等待连接，2s 后超时

    //建议总是阻塞等待，写起来省事
}

//客户端 成功连接函数  READY_OP(1)    连接成功后，设置界面的状态
void ClientWindow::connected_successfully() {
    ui->connect_button->setEnabled(false);
    ui->disconnect_button->setEnabled(true);
    ui->send_button->setEnabled(true);
    ui->port_edit->setReadOnly(true);
    ui->ip_edit->setText("Connected");
    ui->ip_edit->setReadOnly(true);
    socket->send(NetworkData(OPCODE::READY_OP, "", "", ""));
    //连接成功后，发送一个消息给服务器，告诉服务器我已经准备好了,这不是网络中必须的操作，但是在游戏中，我们可能会规定这样的行为
}

//客户端 断开连接函数  恢复最初设置  LEAVE_OP
void ClientWindow::disconnectFromServer() {
    socket->send(NetworkData(OPCODE::LEAVE_OP, "", "", ""));
    //客户端断开连接时要发送一个消息给服务器，这不是网络中必须的操作，这只是方便服务端知道客户端正常离开了（而不是异常断开）
    socket->bye();
    // 在这个网络包里，断开只要调用 bye 就行了，但是在游戏的联机协议里，我们可能会规定其他行为，比如上面说的发送一个消息
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
    ui->send_button->setEnabled(false);
    ui->port_edit->setReadOnly(false);
    ui->ip_edit->setReadOnly(false);
    ui->ip_edit->setText(ip);
}

//客户端要求再来一局
void ClientWindow::reconnect()
{
    socket->hello(ip,port);
    if(!this->socket->base()->waitForConnected(2000)){
        ui->StatusLabel->setText("Connected fail");
    }
    else
    {
        ui->StatusLabel->setText("Connected");
    }
}
//重设姓名、IP和端口并连接
void ClientWindow::reset()
{
    MyName=ui->NameEdit->text();
    ip=ui->ip_edit->text();
    port=ui->port_edit->text().toInt();
    reconnect();
}
//重设姓名
void ClientWindow::resetName()
{
    MyName=ui->NameEdit->text();
}



//客户端 发送信息函数 CHAT_OP
void ClientWindow::sendMessage() {
    QString message = ui->send_edit->text();
    socket->send(NetworkData(OPCODE::CHAT_OP, "", message, ""));//发送消息给服务端
    ui->send_edit->clear();
}

//客户端信息处理函数
void ClientWindow::receiveMessage(NetworkData data) {
    ui->receive_edit->setText(data.data2);//data 是收到的消息，我们显示出来

    switch (data.op){

    case OPCODE::READY_OP:

    }
}

ClientWindow::~ClientWindow() {
    delete ui;
    delete socket;
}
