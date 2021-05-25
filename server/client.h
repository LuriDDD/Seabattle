#ifndef CLIENT_H
#define CLIENT_H
#include <QtNetwork>
#include <QTcpSocket>

enum STATUS {
    START_NEW_GAME = 0,
    SEARCHING_FOR_ENEMY,
    PLACING_SHIPS,
    WAITING_FOR_ENEMY,
    MAKING_STEP,
    WAITNG_STEP,
    VICTORY,
    DEFEAT

};

enum Cell {
    CLEAR = 0,
    SHIP,
    DOT,
    REDSHIP
};

class Client
{
public:
    Client();
    void check_ship(int x, int y) {
        if (ships[x][y] != CLEAR) {
            ships_to_send[x][y] = REDSHIP;
            ships[x][y] = REDSHIP;
            killed_ships++;
        }
        else {
            ships_to_send[x][y] = DOT;
            ships[x][y] = DOT;
        }
    }
    QString field_for_enemy() {
        QString res;
        for (int i =0; i< 10; i++) {
            for (int j =0; j<10; j++) {
                res += QString::number(int(ships_to_send[i][j]));
            }
        }
        return res;
    }
    QString field() {
        QString res;
        for (int i =0; i< 10; i++) {
            for (int j =0; j<10; j++) {
                res += QString::number(int(ships[i][j]));
            }
        }
        return res;
    }
    bool killed_ship(int x, int y, int x1, int y1);
    void mark_killed_ship(int x, int y, int x1, int y1);
    int killed_ships;
    QTcpSocket* socket;
    QTcpSocket* enemy;
    STATUS status;
    Cell ships[10][10];
    Cell ships_to_send[10][10];

};

#endif // CLIENT_H
