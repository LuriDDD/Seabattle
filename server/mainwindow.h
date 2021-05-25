#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <client.h>
#include <QLineEdit>

namespace Ui {
    class MainWindow;
}

class QTcpServer;



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_starting_clicked();
    void on_stoping_clicked();
    void newuser();
    void slotReadClient();
    void todisconnect();
    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    int server_status;
//    QMap<int,QTcpSocket *> SClients;
    QMap<int,Client> SClients;
};

#endif // MAINWINDOW_H
