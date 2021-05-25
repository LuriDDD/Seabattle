#include "field.h"

Field::Field()
{

}

Field::Field(int a, int b, int c, int d)
    : x1{a}, y1{b}, x2{c}, y2{d}, to_paint{false}
{
    for (int i =0; i < 10; i++){
        for(int j =0; j < 10; j++) {
            ships[i][j]= CLEAR;
        }
    }

}
QPoint Field::getcell(const QPoint &coor) {
    QPoint p;
    p.setX((coor.x() - 1-x1)/(double(x2-x1)/10));
    p.setY((coor.y() - 1-y1)/(double(y2-y1)/10));
    return p;
}

QPoint Field::getcoord(const QPoint &coor) {
    QPoint p;
    p.setX(x1+coor.x()*(x2-x1)/10);
    p.setY(y1+coor.y()*(y2-y1)/10);
    return p;
}

bool Field::inside(const QPoint &coor) {
    if (x1 <= coor.x() and coor.x() <= x2 and  y1 <= coor.y() and coor.y() <= y2)
        return true;
    return false;
}

void Field::check_to_paint(const QPoint &coor) {
    if (coor.x()-1 >=0 and coor.y()-1>=0) {
        if (ships[coor.x()-1][coor.y()-1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x() >=0 and coor.y()-1>=0) {
        if (ships[coor.x()][coor.y()-1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x() + 1 <=9 and coor.y()-1>=0) {
        if (ships[coor.x()+1][coor.y()-1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x()+1 <=9 and coor.y()>=0) {
        if (ships[coor.x()+1][coor.y()] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x()+1 <=9 and coor.y()+1<=9) {
        if (ships[coor.x()+1][coor.y()+1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x() >=0 and coor.y()+1<=9) {
        if (ships[coor.x()][coor.y()+1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x()-1 >=0 and coor.y()+1<=9) {
        if (ships[coor.x()-1][coor.y()+1] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    if (coor.x()-1 >=0 and coor.y()>=0) {
        if (ships[coor.x()-1][coor.y()] != CLEAR) {
            to_paint = false;
            return;
        }
    }
    to_paint = true;
    return;
}
