#include "Bullet.h"

void Bullet::Draw()const {
    setlinecolor(RGB(255, 155, 50));
    setfillcolor(RGB(200, 75, 10));
    fillcircle(bullet_pos.x, bullet_pos.y, RADIUS);
}
