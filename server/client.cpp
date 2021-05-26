#include "client.h"

Client::Client()
    : socket{nullptr},
      enemy{nullptr},
      status{START_NEW_GAME},
      killed_ships{0}
{
    for (int i =0; i < 10; i++){
        for(int j =0; j < 10; j++) {
            ships[i][j]= CLEAR;
            ships_to_send[i][j] = CLEAR;
        }
    }

}


bool Client::killed_ship(int x, int y, int x1, int y1) {

    bool res = (ships[x][y] == SHIP)? false: true;
    if(!res) return res;
    if (x >=0 and y-1>=0  and y-1 != y1) {
        if (ships[x][y-1] == SHIP or ships[x][y-1] == REDSHIP)
            res = killed_ship(x, y-1, x, y);
        if(!res) return res;
    }
    if (x +1 <=9 and y>=0 and x+1 != x1) {
        if (ships[x+1][y] == SHIP or ships[x+1][y] == REDSHIP)
            res = killed_ship(x+1, y, x, y);
        if(!res) return res;
    }
    if (x >=0 and y+1<=9 and y+1 != y1) {
        if(ships[x][y+1] == SHIP or ships[x][y+1] == REDSHIP)
            res = killed_ship(x, y+1, x, y);
        if(!res) return res;
    }
    if (x -1 >=0 and y>=0 and x-1 !=x1) {
        if (ships[x-1][y] == SHIP or ships[x-1][y] == REDSHIP)
            res = killed_ship(x-1, y, x, y);
        if(!res) return res;
    }
    return res;
}

void Client::mark_killed_ship(int x, int y, int x1, int y1) {
    if (x -1 >=0 and y-1>=0 ) {
        if (ships[x-1][y-1] == CLEAR) {
            ships[x-1][y-1] = DOT;
            ships_to_send[x-1][y-1] = DOT;
        }

    }
    if (x >=0 and y-1>=0 and y-1 != y1) {
        if (ships[x][y-1] == REDSHIP) {
            mark_killed_ship(x,y-1, x,y);
        }
        if (ships[x][y-1] == CLEAR) {
            ships[x][y-1] = DOT;
            ships_to_send[x][y-1] = DOT;
        }

    }
    if (x + 1 <=9 and y-1>=0) {
        if (ships[x+1][y-1] == CLEAR) {
            ships[x+1][y-1] = DOT;
            ships_to_send[x+1][y-1] = DOT;
        }

    }
    if (x +1 <=9 and y>=0 and x+1 != x1) {
        if (ships[x+1][y] == REDSHIP) {
            mark_killed_ship(x+1,y, x,y);
        }
        if (ships[x+1][y] == CLEAR) {
            ships[x+1][y] = DOT;
            ships_to_send[x+1][y] = DOT;
        }

    }
    if (x +1 <=9 and y+1<=9) {
        if (ships[x+1][y+1] == CLEAR) {
            ships[x+1][y+1] = DOT;
            ships_to_send[x+1][y+1] = DOT;
        }

    }
    if (x >=0 and y+1<=9 and y+1 != y1) {
        if (ships[x][y+1] == REDSHIP) {
            mark_killed_ship(x,y+1, x,y);
        }
        if (ships[x][y+1] == CLEAR) {
            ships[x][y+1] = DOT;
            ships_to_send[x][y+1] = DOT;
        }

    }
    if (x -1 >=0 and y+1<=9) {
        if (ships[x-1][y+1] == CLEAR) {
            ships[x-1][y+1] = DOT;
            ships_to_send[x-1][y+1] = DOT;
        }

    }
    if (x -1 >=0 and y>=0 and x-1 !=x1) {
        if (ships[x-1][y] == REDSHIP) {
            mark_killed_ship(x-1,y, x, y);
        }
        if (ships[x-1][y] == CLEAR) {
            ships[x-1][y] = DOT;
            ships_to_send[x-1][y] = DOT;
        }
    }
}
