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
    const int ENEMY_SPEED = 2;// �����ٶ�
    const int ENEMY_WIDTH = 80;// ���˿��
    const int ENEMY_HEIGHT = 80;// ���˸߶�
    const int SHADOW_WIDTH = 48;// ��Ӱ���

    IMAGE img_shadow;// ��Ӱͼ��
    Animation* anim_enemy_left;
    Animation* anim_enemy_right;
    POINT enemy_pos = { 0,0 };// ���˳�ʼλ��

    bool facing_left = false;// �����˶���־λ
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