#include "Enemy.h"

extern Atlas* atlas_enemy_left;
extern Atlas* atlas_enemy_right;

// ͸���Ȼ��
inline void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    // ��ָ��λ�û��ƾ���͸���ȵ�ͼ��
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255, AC_SRC_ALPHA });
}

Enemy::Enemy() {
    loadimage(&img_shadow, _T("img/shadow_enemy.png"));
    anim_enemy_left = new Animation(atlas_enemy_left, 45);
    anim_enemy_right = new Animation(atlas_enemy_right, 45);

    // �������ɱ߽�
    enum class SpawnEdge {
        Up = 0,
        Down,
        Left,
        Right
    };
    // �����˷����ڵ�ͼ��߽����λ��
    SpawnEdge edge = (SpawnEdge)(rand() % 4);

    switch (edge) {
    case SpawnEdge::Up:
        enemy_pos.x = rand() % WINDOW_WIDTH;
        enemy_pos.y = -ENEMY_HEIGHT;
        break;
    case SpawnEdge::Down:
        enemy_pos.x = rand() % WINDOW_WIDTH;
        enemy_pos.y = WINDOW_HEIGHT;
        break;
    case SpawnEdge::Left:
        enemy_pos.x = -ENEMY_WIDTH;
        enemy_pos.y = rand() % WINDOW_HEIGHT;
        break;
    case SpawnEdge::Right:
        enemy_pos.x = WINDOW_WIDTH;
        enemy_pos.y = rand() % WINDOW_HEIGHT;
        break;
    default:
        break;
    }
}

void Enemy::Hurt() {
    mIsAlive = false;// һ����ɱ
}

// �����
bool Enemy::CheckAlive() {
    return mIsAlive;
}

// ���������ӵ���������ײ
bool Enemy::CheckBulletCollision(const Bullet& bullet) {
    bool is_overlap_x = bullet.bullet_pos.x > enemy_pos.x && bullet.bullet_pos.x < (enemy_pos.x + ENEMY_WIDTH);
    bool is_overlap_y = bullet.bullet_pos.y > enemy_pos.y && bullet.bullet_pos.y < (enemy_pos.y + ENEMY_WIDTH);
    return is_overlap_x && is_overlap_y;
}

// ����������ҷ�������ײ
bool Enemy::CheckPlayerCollision(const Player& player) {
    // ����������ĵ���������ײ�䣬����Ϸʧ��
    const POINT& enemy_center_point = { enemy_pos.x + ENEMY_WIDTH / 2,enemy_pos.y + ENEMY_HEIGHT / 2 };// �������ĵ�
    bool is_overlap_x = enemy_center_point.x > player.GetPosition().x && enemy_center_point.x < (player.GetPosition().x + ENEMY_WIDTH);// ENEMY_WIDTH����PLAYER_WIDTH
    bool is_overlap_y = enemy_center_point.y > player.GetPosition().y && enemy_center_point.y < (player.GetPosition().y + ENEMY_HEIGHT);// ENEMY_HEIGHT����PLAYER_HEIGHT
    return is_overlap_x && is_overlap_y;
}

void Enemy::Move(const Player& player) {
    const POINT& player_position = player.GetPosition();
    int dir_x = player_position.x - enemy_pos.x;
    int dir_y = player_position.y - enemy_pos.y;
    double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);

    if (dir_len != 0) {// ����enemyб���ƶ��ٶȱ���bug
        double normalized_x = dir_x / dir_len;
        double normalized_y = dir_y / dir_len;
        enemy_pos.x += (int)(ENEMY_SPEED * normalized_x);
        enemy_pos.y += (int)(ENEMY_SPEED * normalized_y);
    }
    if (dir_x < 0)
        facing_left = true;
    else if (dir_x > 0)
        facing_left = false;
}

void Enemy::Draw(int delta) {
    int pos_shadow_x = enemy_pos.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
    int pos_shadow_y = enemy_pos.y + ENEMY_HEIGHT - 35;
    putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

    if (facing_left)
        anim_enemy_left->Play(enemy_pos.x, enemy_pos.y, delta);
    else
        anim_enemy_right->Play(enemy_pos.x, enemy_pos.y, delta);
}

const POINT& Enemy::GetPosition() const {
    return enemy_pos;
}
