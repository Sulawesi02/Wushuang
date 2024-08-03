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
    const int PLAYER_SPEED = 5;// 玩家速度

    IMAGE img_shadow;// 阴影图像
    Animation* anim_player_left;
    Animation* anim_player_right;
    POINT player_pos = { 500,500 };// 玩家初始位置

    bool is_move_up = false;
    bool is_move_down = false;
    bool is_move_left = false;
    bool is_move_right = false;

public:
    const int PLAYER_WIDTH = 80;// 玩家宽度
    const int PLAYER_HEIGHT = 80;// 玩家高度
    const int SHADOW_WIDTH = 32;// 阴影宽度

    Player();
    ~Player() = default;

    void ProcessEvent(const ExMessage& msg);
    void Move();
    void Draw(int delta);
    const POINT& GetPosition() const;
};

#endif 