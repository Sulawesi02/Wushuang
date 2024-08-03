#ifndef ANIMATION_H
#define ANIMATION_H

#include<graphics.h>
#include<string>
#include<vector>
#include "Atlas.h"

// �����ࡣ�������Ĳ��ţ�����֡�ĸ��ºͼ�ʱ
class Animation {
    int timer = 0;// ��ʱ��
    int idx_frame = 0;//֡����
    int interval_ms = 0;//֡���
    Atlas* anim_atlas;

public:
    Animation(Atlas* atlas, int interval);
    ~Animation();

    void Play(int x, int y, int delta);
};

#endif 