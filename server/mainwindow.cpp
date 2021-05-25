#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->centralWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
    server_status=0;
}

void MainWindow::on_starting_clicked()
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newuser()));
    if (!tcpServer->listen(QHostAddress::Any, 3333) && server_status==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer->errorString());
        ui->textinfo->append(tcpServer->errorString());
    } else {
        server_status=1;
        qDebug() << tcpServer->isListening() << "TCPSocket listen on port";
        ui->textinfo->append(QString::fromUtf8("Сервер запущен!"));
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void MainWindow::on_stoping_clicked()
{
    if(server_status==1){
        foreach(int i,SClients.keys()){
            QTextStream os(SClients[i].socket);
            os.setAutoDetectUnicode(true);
            os << QDateTime::currentDateTime().toString() << "\n";
            SClients[i].socket->close();
            SClients.remove(i);
        }
        tcpServer->close();
        ui->textinfo->append(QString::fromUtf8("Сервер остановлен!"));
        qDebug() << QString::fromUtf8("Сервер остановлен!");
        server_status=0;
    }
}


void MainWindow::newuser()
{
    if(server_status==1){
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer->nextPendingConnection();
//        ui->textinfo->append(clientSocket->peerAddress().toString());
        int idusersocs=clientSocket->socketDescriptor();
        ui->textinfo->append(QString::number(idusersocs));
        Client newuser;
        newuser.socket = clientSocket;
        connect(clientSocket,SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        connect(clientSocket,SIGNAL(disconnected()),this, SLOT(todisconnect()));
        newuser.status = SEARCHING_FOR_ENEMY;
        for (auto it=SClients.begin(); it != SClients.end(); it++) {
            if ((*it).enemy == nullptr) {
                clientSocket->write("server:2:0");
                newuser.enemy=(*it).socket;
                newuser.status = PLACING_SHIPS;

                (*it).socket->write("server:2:0");
                (*it).enemy = clientSocket;
                (*it).status = PLACING_SHIPS;
                break;
            }
        }
        if (newuser.status == SEARCHING_FOR_ENEMY) clientSocket->write("server:1:0");
        SClients[idusersocs] = newuser;
    }
}

void MainWindow::slotReadClient()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();
    int idusersocsenemy=SClients[idusersocs].enemy->socketDescriptor();

//    QTextStream os(clientSocket);
//    os.setAutoDetectUnicode(true);
//    os << "HTTP/1.0 200 Ok\r\n"
//          "Content-Type: text/html; charset=\"utf-8\"\r\n"
//          "\r\n"
//          "<h1>Nothing to see here</h1>\n"
//          << QDateTime::currentDateTime().toString() << "\n";
    QString response = clientSocket->readAll();
//    ui->textinfo->append("ReadClient:"+response+"\n\r");
    QRegularExpression rx("client:(\\d+)");
    QRegularExpressionMatch rxm = rx.match(response);
//    ui->textinfo->append(response);
    QString answer;
    switch (SClients[idusersocs].status) {
    case START_NEW_GAME:
        break;
    case SEARCHING_FOR_ENEMY:
        break;
    case PLACING_SHIPS:
    {
        for(int i =0; i <rxm.captured(1).size(); i++) {
            SClients[idusersocs].ships[i/10][i%10] = Cell(QString(rxm.captured(1)[i]).toInt());
        }
        if (SClients[idusersocsenemy].status == WAITING_FOR_ENEMY) {
            SClients[idusersocs].enemy->write("server:4:0");
            SClients[idusersocsenemy].status = MAKING_STEP;

            SClients[idusersocs].socket->write("server:5:0");
            SClients[idusersocs].status = WAITNG_STEP;
        }
        else {
            SClients[idusersocs].socket->write("server:3:0");
            SClients[idusersocs].status = WAITING_FOR_ENEMY;
        }
    }
        break;
    case WAITING_FOR_ENEMY:
        break;
    case MAKING_STEP:
    {
        QString answer, answer_for_enemy;
        SClients[idusersocsenemy].check_ship(QString(rxm.captured(1)[0]).toInt(), QString(rxm.captured(1)[1]).toInt());
        if(SClients[idusersocsenemy].ships[QString(rxm.captured(1)[0]).toInt()][QString(rxm.captured(1)[1]).toInt()] == REDSHIP) {
            if (SClients[idusersocsenemy].killed_ships == 20) {
                answer = "server:6:";
                answer_for_enemy= "server:7:";
                SClients[idusersocs].status = VICTORY;
                SClients[idusersocsenemy].status = DEFEAT;
            }
            else {
                answer = "server:4:";
                answer_for_enemy= "server:5:";
                SClients[idusersocs].status = MAKING_STEP;
                SClients[idusersocsenemy].status = WAITNG_STEP;
            }

            if (SClients[idusersocsenemy].killed_ship(QString(rxm.captured(1)[0]).toInt(),QString(rxm.captured(1)[1]).toInt(), -5, -5)) {
                SClients[idusersocsenemy].mark_killed_ship(QString(rxm.captured(1)[0]).toInt(),QString(rxm.captured(1)[1]).toInt(), -5, -5);
            }
        }
        else {
            answer = "server:5:";
            answer_for_enemy= "server:4:";
            SClients[idusersocs].status = WAITNG_STEP;
            SClients[idusersocsenemy].status = MAKING_STEP;
        }
        answer += SClients[idusersocsenemy].field_for_enemy();
        SClients[idusersocs].socket->write(answer.toLocal8Bit());
        answer_for_enemy += SClients[idusersocsenemy].field();
        SClients[idusersocsenemy].socket->write(answer_for_enemy.toLocal8Bit());
        if(SClients[idusersocs].status == VICTORY) {
            SClients[idusersocs].socket->close();
            SClients[idusersocsenemy].socket->close();
            SClients.remove(idusersocs);
            SClients.remove(idusersocsenemy);
        }
    }
        break;
    case WAITNG_STEP:
        break;
    }
    // Если нужно закрыть сокет
//    clientSocket->close();
//    SClients.remove(idusersocs);
}

void MainWindow::on_pushButton_clicked()
{
    QString message = ui->messagetext->text();
    QRegularExpression rx("(\\w+):(\\d+):(.*)");
    QRegularExpressionMatch rxm = rx.match(message);
    ui->textinfo->append(rxm.captured(1));
    if (rxm.captured(1) == "message") {
        if (SClients.find(rxm.captured(2).toInt()) != SClients.end()) {
            QString text = "message:";
            text +=rxm.captured(3);
            SClients[rxm.captured(2).toInt()].socket->write(text.toUtf8());
        }
    }

}

void MainWindow::todisconnect() {
    QTcpSocket* client = (QTcpSocket*)sender();
    for (auto it = SClients.begin(); it != SClients.end(); it++) {
        if ((*it).socket == client) {
            (*it).socket->close();
            client = (*it).enemy;
            SClients.remove(it.key());
            break;
        }
    }
    if (client != nullptr) {
        for (auto it = SClients.begin(); it != SClients.end(); it++) {
            if ((*it).socket == client) {
                (*it).socket->close();
                SClients.remove(it.key());
                break;
            }
        }
    }
    ui->textinfo->append(QString::fromUtf8("Соединение прервано"));
//    if (SClients[id].enemy != nullptr) {
//        SClients[SClients[id].enemy->socketDescriptor()].socket->close();
//        SClients.remove(SClients[id].enemy->socketDescriptor());
//    }
//    SClients[id].socket->close();
//    SClients.remove(id);
}
