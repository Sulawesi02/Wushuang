#ifndef PLAYER_H
#define PLAYER_H

#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>
#include "Animation.h"

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

class Player {
    const int PLAYER_SPEED = 5;// ����ٶ�

    IMAGE img_shadow;// ��Ӱͼ��
    Animation* anim_player_left;
    Animation* anim_player_right;
    POINT player_pos = { 500,500 };// ��ҳ�ʼλ��

    bool is_move_up = false;
    bool is_move_down = false;
    bool is_move_left = false;
    bool is_move_right = false;

public:
    const int PLAYER_WIDTH = 80;// ��ҿ��
    const int PLAYER_HEIGHT = 80;// ��Ҹ߶�
    const int SHADOW_WIDTH = 32;// ��Ӱ���

    Player();
    ~Player() = default;

    void ProcessEvent(const ExMessage& msg);
    void Move();
    void Draw(int delta);
    const POINT& GetPosition() const;
};

#endif 