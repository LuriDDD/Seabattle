#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , status(START_NEW_GAME)
    , orientation(true)
    , server{nullptr}
{
    amount_of_ships.fill(0, 4);
    setMouseTracking(true);
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
    delete server;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

    QString response;
//    response += QString::number(pos.x());
//    response += ":";
//    response += QString::number(pos.y());
//    server->write(response.toLocal8Bit());
//    QRegularExpression rx("(\\d+):(\\d+)");
//    if (!server->waitForReadyRead(5000)) return;
//    response = server->readAll();
//    QRegularExpressionMatch rxm = rx.match(response);
//    pos.setX(rxm.captured(1).toInt());
//    pos.setY(rxm.captured(2).toInt());
    switch (status) {
    case START_NEW_GAME:
        break;
    case SEARCHING_FOR_ENEMY:
        break;
    case PLACING_SHIPS:
        if(event->button() == Qt::RightButton) {
            orientation = 1 - orientation;
            return;
        }
        if (my_field.to_paint) {
            int cnt = 0;
            for (auto i = 3; i >= 0; i--) {
                if (amount_of_ships[i] + i != 4) {
                    cnt = i+ 1;
                    amount_of_ships[i]++;
                    break;
                }
            }
           for (int i =0; i < cnt; i++)
               my_field.ships[ship.x()+((orientation)?i:0)][ship.y()+((!orientation)?i:0)] = SHIP;
           if (amount_of_ships[0] == 4) {
               response = "client:";
               for (int i =0; i < 10; i++) {
                   for(int j =0; j < 10; j++) {
                       response += QString::number(my_field.ships[i][j]);
                   }
               }
               server->write(response.toLocal8Bit());
               server->waitForReadyRead();
           }
        }
        break;
    case WAITING_FOR_ENEMY:
        break;
    case MAKING_STEP:
        if (enemy_field.inside(event->pos())) {
            QPoint pos = enemy_field.getcell(event->pos());
            if(enemy_field.ships[pos.x()][pos.y()] == CLEAR) {
                QString cell = "client:";
                cell += QString::number(pos.x());
                cell += QString::number(pos.y());
                server->write(cell.toLocal8Bit());
                server->waitForReadyRead();
            }
        }
        break;
    case WAITNG_STEP:
        break;
    }


     this->update();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, this->menuBar()->geometry().height(), images.getimage("field"));


    for (int i =0; i < 10; i++) {
        for(int j =0; j < 10; j++) {
            if (my_field.ships[i][j] == CLEAR) continue;
            if (my_field.ships[i][j] == SHIP) painter.drawImage( my_field.getcoord(QPoint(i, j)), images.getimage("full"));
            if (my_field.ships[i][j] == DOT) painter.drawImage( my_field.getcoord(QPoint(i, j)), images.getimage("dot"));
            if (my_field.ships[i][j] == REDSHIP) painter.drawImage( my_field.getcoord(QPoint(i, j)), images.getimage("redfull"));
        }
    }
    for (int i =0; i < 10; i++) {
        for(int j =0; j < 10; j++) {
            if (enemy_field.ships[i][j] == CLEAR) continue;
            if (enemy_field.ships[i][j] == SHIP) painter.drawImage( enemy_field.getcoord(QPoint(i, j)), images.getimage("full"));
            if (enemy_field.ships[i][j] == DOT) painter.drawImage( enemy_field.getcoord(QPoint(i, j)), images.getimage("dot"));
            if (enemy_field.ships[i][j] == REDSHIP) painter.drawImage( enemy_field.getcoord(QPoint(i, j)), images.getimage("redfull"));
        }
    }
    switch (status) {
    case START_NEW_GAME:
        ui->statuslabel->setText("Статус: Начните новую игру");
        break;
    case SEARCHING_FOR_ENEMY:
        ui->statuslabel->setText("Статус: Идет поиск соперника");
        break;
    case PLACING_SHIPS:
    {
        ui->statuslabel->setText("Статус: Расставляйте корабли");
        int cnt = 0;
        for (auto i = 3; i >= 0; i--) {
            if (amount_of_ships[i] + i != 4) {
            cnt = i;
            cnt++;
            break;
            }
        }
        if (my_field.to_paint) {
            for (int i =0; i < cnt; i++)
                painter.drawImage( my_field.getcoord(QPoint(ship.x()+((orientation)?i:0),
                                                            ship.y() + ((!orientation)?i:0))), images.getimage("redfull"));
        }
    }
        break;
    case WAITING_FOR_ENEMY:
        ui->statuslabel->setText("Статус: Ожидание соперника");
        break;
    case MAKING_STEP:
        ui->statuslabel->setText("Статус: Ваш ход");
        break;
    case WAITNG_STEP:
        ui->statuslabel->setText("Статус: Ход соперника");
        break;
    case VICTORY:
        ui->statuslabel->setText("Статус: Победа");
        break;
    case DEFEAT:
        ui->statuslabel->setText("Статус: Поражение");
        break;
    case DISCONNECTED:
        ui->statuslabel->setText("Статус: Соперник отключился");
        break;
    }
//    if (enemy_field.inside(ship)) {
//       painter.drawImage(enemy_field.getcoord(enemy_field.getcell(ship)), images.getimage("redfull"));
//    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
//    if (my_field.inside(event->pos())) {
//       ship = my_field.getcoord(my_field.getcell(event->pos()));
//        qDebug("%d, %d", event->pos().x(), event->pos().y());
//    }
//    if (enemy_field.inside(event->pos())) {
//       ship = enemy_field.getcoord(enemy_field.getcell(event->pos()));
//        qDebug("%d, %d", event->pos().x(), event->pos().y());
//    }
    switch (status) {
    case START_NEW_GAME:
        break;
    case SEARCHING_FOR_ENEMY:
        break;
    case PLACING_SHIPS:
        if (my_field.inside(event->pos())) {
            QPoint pos = my_field.getcell(event->pos());
            to_place_ships(pos);
            ship = pos;
        }
        else my_field.to_paint = false;
        this->update();
        break;
    case WAITING_FOR_ENEMY:
        break;
    case MAKING_STEP:
        break;
    case WAITNG_STEP:
        break;
    case DISCONNECTED:
        break;
    }
}

void MainWindow::on_actionNew_Game_triggered()
{


    if (server != nullptr and server->isOpen()) server->close();
    server = new QTcpSocket;
    //18.156.108.157 192.168.1.180
    status = START_NEW_GAME;
    orientation = true;
    ship = QPoint();
    server->connectToHost("18.156.108.157", 3333);
    this->connect(server, SIGNAL(readyRead()), SLOT(server_read()));
    this->connect(server,SIGNAL(disconnected()), SLOT(todisconnect()));
    my_field = Field(40, 62, 255, 277);
    enemy_field = Field(321, 62, 536, 277);
    amount_of_ships.fill(0, 4);
    ui->centralwidget->setMouseTracking(true);
}

void MainWindow::to_place_ships(QPoint& pos) {
    int cnt=0;
    for (auto i = 3; i >= 0; i--) {
        if (amount_of_ships[i] + i != 4) {
            cnt = i;
            cnt++;
            break;
        }
    }
    if (orientation) {
        if (pos.x() + cnt > 9) pos.setX(10-cnt);
        qDebug() << pos.x() << " " << pos.y();
    }
    else {
        if (pos.y() + cnt > 9) pos.setY(10-cnt);
        qDebug() << pos.x() << " " << pos.y();
    }
    my_field.to_paint = true;
    for (int i =0; i < cnt; i++) {
        my_field.check_to_paint(QPoint(ship.x()+((orientation)?i:0), ship.y() + ((!orientation)?i:0)));
        if (!my_field.to_paint) break;
    }

}

void MainWindow::server_read() {
    QString pack;
    pack = server->readLine();
    QRegularExpression message("(\\w+):(.*)");
    QRegularExpressionMatch remessage = message.match(pack);
    QMessageBox msg;
    if (remessage.captured(1) == "message") {
    if (remessage.captured(2) == "girl") {
        msg.setText("<img src=\":/unnamed.jpg\">");
        msg.exec();
    }
    if (remessage.captured(2) == "sosi") {
        msg.setText("<img src=\":/pososi.jpg\">");
        msg.exec();
    }
    return;
    }
//    QMessageBox::information(this, "Message", pack);
    QRegularExpression rx("server:([0-9]):([0-3]+)");
    QRegularExpressionMatch rxm = rx.match(pack);
    switch (status) {
    case START_NEW_GAME:
        break;
    case SEARCHING_FOR_ENEMY:
        break;
    case PLACING_SHIPS:
        break;
    case WAITING_FOR_ENEMY:
        break;
    case MAKING_STEP:
        for (int i = 0; i < 100; i++) {
            enemy_field.ships[i/10][i%10] = Cell(QString(rxm.captured(2)[i]).toInt());
        }
        break;
    case WAITNG_STEP:
        for (int i = 0; i < 100; i++) {
            my_field.ships[i/10][i%10] = Cell(QString(rxm.captured(2)[i]).toInt());
        }
        break;
    }
    status = STATUS(rxm.captured(1).toInt());
    if (status == VICTORY or status == DEFEAT) {
        if (status == VICTORY) {
            QMessageBox::about(this, "Победа", "Поздравляю, вы победили!");
        }
        else {
            QMessageBox::about(this, "Поражение", "К сожалению, вы проиграли!");
        }
        server->close();
    }
    this->update();

}

void MainWindow::todisconnect() {
    server->close();
    if (status != VICTORY and status != DEFEAT)
        status = DISCONNECTED;
}

