#ifndef ANIMATION_H
#define ANIMATION_H

#include<graphics.h>
#include<string>
#include<vector>
#include "Atlas.h"

// 动画类。管理动画的播放，包括帧的更新和计时
class Animation {
    int timer = 0;// 计时器
    int idx_frame = 0;//帧索引
    int interval_ms = 0;//帧间隔
    Atlas* anim_atlas;

public:
    Animation(Atlas* atlas, int interval);
    ~Animation();

    void Play(int x, int y, int delta);
};

#endif 