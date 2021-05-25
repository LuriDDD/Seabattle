#ifndef FIELD_H
#define FIELD_H
#include <QVector>
#include <QPoint>
enum Cell {
    CLEAR = 0,
    SHIP,
    DOT,
    REDSHIP
};

class Field
{
public:
    Field();
    Field(int a, int b, int c, int d);
    QPoint getcell(const QPoint& coor);
    QPoint getcoord(const QPoint& coor);
    bool inside(const QPoint& coor);
    Cell ships[10][10];
    bool to_paint;
    void check_to_paint(const QPoint& coor);
private:
    int x1, y1, x2, y2;

};

#endif // FIELD_H
