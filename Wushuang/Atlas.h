#ifndef ATLAS_H
#define ATLAS_H

#include<graphics.h>
#include<string>
#include<vector>

// ͼ���ࡣ������һ������������Ҷ�����6֡ͼ��
class Atlas {

public:
    std::vector<IMAGE*>frame_list;

    Atlas(LPCTSTR path, int num);
    ~Atlas();
};

#endif 