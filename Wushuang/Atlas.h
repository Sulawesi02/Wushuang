#ifndef ATLAS_H
#define ATLAS_H

#include<graphics.h>
#include<string>
#include<vector>

// 图集类。管理玩家或敌人向左或向右动作的6帧图像
class Atlas {

public:
    std::vector<IMAGE*>frame_list;

    Atlas(LPCTSTR path, int num);
    ~Atlas();
};

#endif 