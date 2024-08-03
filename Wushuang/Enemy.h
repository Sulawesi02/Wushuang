#ifndef ENEMY_H
#define ENEMY_H

#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>
#include "Player.h"
#include "Bullet.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

class Enemy {
    const int ENEMY_SPEED = 2;// 敌人速度
    const int ENEMY_WIDTH = 80;// 敌人宽度
    const int ENEMY_HEIGHT = 80;// 敌人高度
    const int SHADOW_WIDTH = 48;// 阴影宽度

    IMAGE img_shadow;// 阴影图像
    Animation* anim_enemy_left;
    Animation* anim_enemy_right;
    POINT enemy_pos = { 0,0 };// 敌人初始位置

    bool facing_left = false;// 朝左运动标志位
    bool mIsAlive = true;

public:

    Enemy();
    ~Enemy();

    void Hurt();
    bool CheckAlive();
    bool CheckBulletCollision(const Bullet& bullet);
    bool CheckPlayerCollision(const Player& player);
    void Move(const Player& player);
    void Draw(int delta);
    const POINT& GetPosition() const;

};

#endif