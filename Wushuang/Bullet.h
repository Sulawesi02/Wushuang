#ifndef BULLET_H
#define BULLET_H

#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>

class Bullet {
    const int RADIUS = 10;

public:
    POINT bullet_pos = { 0,0 };

    Bullet();
    ~Bullet();

    void Draw()const;

};

#endif 