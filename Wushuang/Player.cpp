#include "Player.h"

extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;

// 透明度混合
inline void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    // 在指定位置绘制具有透明度的图像
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255, AC_SRC_ALPHA });
}

Player::Player() {
    loadimage(&img_shadow, _T("img/shadow_player.png"));
    anim_player_left = new Animation(atlas_player_left, 45);// 动画帧间隔45ms
    anim_player_right = new Animation(atlas_player_right, 45);
}

Player::~Player() = default;

void Player::ProcessEvent(const ExMessage& msg) {
    if (msg.message == WM_KEYDOWN) {
        switch (msg.vkcode) {
        case VK_UP:
            is_move_up = true;
            break;
        case VK_DOWN:
            is_move_down = true;
            break;
        case VK_LEFT:
            is_move_left = true;
            break;
        case VK_RIGHT:
            is_move_right = true;
            break;
        }
    }
    else if (msg.message == WM_KEYUP) {
        switch (msg.vkcode) {
        case VK_UP:
            is_move_up = false;
            break;
        case VK_DOWN:
            is_move_down = false;
            break;
        case VK_LEFT:
            is_move_left = false;
            break;
        case VK_RIGHT:
            is_move_right = false;
            break;
        }
    }
}
void Player::Move() {
    int dir_x = is_move_right - is_move_left;
    int dir_y = is_move_down - is_move_up;
    double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);

    if (dir_len != 0) {// 消除player斜向移动速度变快的bug
        double normalized_x = dir_x / dir_len;
        double normalized_y = dir_y / dir_len;
        player_pos.x += (int)(PLAYER_SPEED * normalized_x);
        player_pos.y += (int)(PLAYER_SPEED * normalized_y);
    }

    if (player_pos.x < 0)player_pos.x = 0;
    if (player_pos.y < 0)player_pos.y = 0;
    if (player_pos.x > WINDOW_WIDTH - PLAYER_WIDTH)player_pos.x = WINDOW_WIDTH - PLAYER_WIDTH;
    if (player_pos.y > WINDOW_HEIGHT - PLAYER_HEIGHT)player_pos.y = WINDOW_HEIGHT - PLAYER_HEIGHT;

}

void Player::Draw(int delta) {
    int pos_shadow_x = player_pos.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
    int pos_shadow_y = player_pos.y + PLAYER_HEIGHT - 8;
    putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

    static bool facing_left = false;
    int dir_x = is_move_right - is_move_left;
    if (dir_x < 0)
        facing_left = true;
    else if (dir_x > 0)
        facing_left = false;
    if (facing_left)
        anim_player_left->Play(player_pos.x, player_pos.y, delta);
    else
        anim_player_right->Play(player_pos.x, player_pos.y, delta);
}

const POINT& Player::GetPosition() const {
    return player_pos;
}
