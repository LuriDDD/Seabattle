#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPixmap>
#include <QPainter>
#include <QImage>
#include <QPoint>
#include <image.h>
#include <field.h>
#include <QTcpSocket>
#include <QString>
#include <QRegularExpressionMatch>
#include <QRegularExpression>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum STATUS {
    START_NEW_GAME = 0,
    SEARCHING_FOR_ENEMY,
    PLACING_SHIPS,
    WAITING_FOR_ENEMY,
    MAKING_STEP,
    WAITNG_STEP,
    VICTORY,
    DEFEAT,
    DISCONNECTED

};
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void mousePressEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);\

private slots:
    void on_actionNew_Game_triggered();
    void server_read();
    void todisconnect();

private:
    void to_place_ships(QPoint& pos);
    Ui::MainWindow *ui;
    QPoint ship;
    bool orientation;
    QVector<int> amount_of_ships;
    STATUS status;
    Image images;
    Field my_field, enemy_field;
    QTcpSocket *server;
};
#endif // MAINWINDOW_H
