#pragma once
#include<iostream>
#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>
using namespace std;


const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75; 


#pragma comment(lib,"MSIMG32.LIB")
// 透明度混合
static void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255, AC_SRC_ALPHA });
}

// 动画
class Animation {
    int timer = 0;// 计时器
    int idx_frame = 0;//帧索引
    int interval_ms = 0;//帧间隔
    vector<IMAGE*>frame_list;
public:
    Animation(LPCTSTR path, int num, int interval) {
        interval_ms = interval;

        TCHAR path_file[256];
        for (size_t i = 0; i < num; i++) {
            _stprintf_s(path_file, path, i);
            IMAGE* frame = new IMAGE();
            loadimage(frame, path_file);
            frame_list.push_back(frame);
        }
    }
    ~Animation() {
        for (size_t i = 0; i < frame_list.size(); i++) {
            delete frame_list[i]; 
        }
    }
    void Play(int x, int y, int delta){
        timer += delta;
        if (timer >= interval_ms){
            idx_frame = (idx_frame + 1) % frame_list.size();
            timer = 0;
        }
        putimage_alpha(x, y, frame_list[idx_frame]);

    }
};

// 子弹
class Bullet {
    const int RADIUS = 10;

public:
    POINT bullet_pos = { 0,0 };

    Bullet() = default;
    ~Bullet() = default;

    void Draw()const {
        setlinecolor(RGB(255, 155, 50));
        setfillcolor(RGB(200, 75, 10));
        fillcircle(bullet_pos.x, bullet_pos.y, RADIUS);
    }

};

//玩家
class Player{

    IMAGE img_shadow;// 阴影图像
    Animation* player_anim_left;// 向左动作帧序列
    Animation* player_anim_right;// 向右动作帧序列

    bool is_move_up = false;
    bool is_move_down = false;
    bool is_move_left = false;
    bool is_move_right = false;

public:
    const int PLAYER_SPEED = 5;// 玩家速度
    const int PLAYER_WIDTH = 80;// 玩家宽度
    const int PLAYER_HEIGHT = 80;// 玩家高度
    const int SHADOW_WIDTH = 32;// 阴影宽度
    POINT player_pos = { 500,500 };// 玩家初始位置

    Player(){
        loadimage(&img_shadow, _T("img/shadow_player.png"));
        player_anim_left = new Animation(_T("img/paimon_left_%d.png"), 6, 45);// 动画由6张图片构成，帧间隔45ms
        player_anim_right = new Animation(_T("img/paimon_right_%d.png"), 6, 45);
    }

    ~Player(){
        delete player_anim_left;
        delete player_anim_right;
    }

    void ProcessEvent(const ExMessage& msg){
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
    void Move(){
        int dir_x = is_move_right - is_move_left;
        int dir_y = is_move_down - is_move_up;
        double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y);

        if (dir_len != 0){// 消除player斜向移动速度变快的bug
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

    void Draw(int delta){
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
            player_anim_left->Play(player_pos.x, player_pos.y, delta);
        else
            player_anim_right->Play(player_pos.x, player_pos.y, delta);
    }

    const POINT& GetPosition() const{
        return player_pos;
    }
};


//敌人
class Enemy{

    IMAGE img_shadow;// 阴影图像
    Animation* enemy_anim_left;// 向左动作帧序列
    Animation* enemy_anim_right; // 向右动作帧序列
    bool facing_left = false;// 朝左运动标志位
    bool mIsAlive = true; 

public:
    const int ENEMY_SPEED = 2;// 敌人速度
    const int ENEMY_WIDTH = 80;// 敌人宽度
    const int ENEMY_HEIGHT = 80;// 敌人高度
    const int SHADOW_WIDTH = 48;// 阴影宽度
    POINT enemy_pos = { 0,0 };// 敌人初始位置

    Enemy(){
        loadimage(&img_shadow, _T("img/shadow_enemy.png"));
        enemy_anim_left = new Animation(_T("img/boar_left_%d.png"), 6, 45);
        enemy_anim_right = new Animation(_T("img/boar_right_%d.png"), 6, 45);

        // 敌人生成边界
        enum class SpawnEdge{
            Up = 0,
            Down,
            Left,
            Right
        };
        // 将敌人放置在地图外边界随机位置
        SpawnEdge edge = (SpawnEdge)(rand() % 4);

        switch (edge){
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

    ~Enemy(){
        delete enemy_anim_left;
        delete enemy_anim_right;
    }

    void Hurt(){
        mIsAlive = false;// 一击必杀
    }

    // 存活检测
    bool CheckAlive(){
        return mIsAlive;
    } 

    // 检测敌人与子弹发生的碰撞
    bool CheckBulletCollision(const Bullet& bullet){
        bool is_overlap_x = bullet.bullet_pos.x > enemy_pos.x && bullet.bullet_pos.x < (enemy_pos.x + ENEMY_WIDTH);
        bool is_overlap_y = bullet.bullet_pos.y > enemy_pos.y && bullet.bullet_pos.y < (enemy_pos.y + ENEMY_WIDTH);
        return is_overlap_x && is_overlap_y;
    }

    // 检测敌人与玩家发生的碰撞
    bool CheckPlayerCollision(const Player& player){
        // 如果敌人中心点进入玩家碰撞箱，则游戏失败
        const POINT& enemy_center_point = { enemy_pos.x + ENEMY_WIDTH / 2,enemy_pos.y + ENEMY_HEIGHT / 2 };// 敌人中心点
        bool is_overlap_x = enemy_center_point.x > player.GetPosition().x && enemy_center_point.x < (player.GetPosition().x + ENEMY_WIDTH);// ENEMY_WIDTH等于PLAYER_WIDTH
        bool is_overlap_y = enemy_center_point.y > player.GetPosition().y && enemy_center_point.y < (player.GetPosition().y + ENEMY_HEIGHT);// ENEMY_HEIGHT等于PLAYER_HEIGHT
        return is_overlap_x && is_overlap_y;
    }

    void Move(const Player& player){
        const POINT& player_position = player.GetPosition();
        int dir_x = player_position.x - enemy_pos.x;
        int dir_y = player_position.y - enemy_pos.y;
        double dir_len = sqrt(dir_x * dir_x + dir_y * dir_y); 

        if (dir_len != 0){// 消除enemy斜向移动速度变快的bug
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

    void Draw(int delta){
        int pos_shadow_x = enemy_pos.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
        int pos_shadow_y = enemy_pos.y + ENEMY_HEIGHT - 35;
        putimage_alpha(pos_shadow_x, pos_shadow_y, &img_shadow);

        if (facing_left)
            enemy_anim_left->Play(enemy_pos.x, enemy_pos.y, delta);
        else
            enemy_anim_right->Play(enemy_pos.x, enemy_pos.y, delta);
    }

    const POINT& GetPosition() const{
        return enemy_pos;
    }

};

// 生成新的敌人
void TryGenerateEnemy(vector<Enemy*>& enemy_list){
    const int INTERVAL = 100;
    static int counter = 0;
    if ((++counter) % INTERVAL == 0)
        enemy_list.push_back(new Enemy());
}

// 更新子弹位置
void UpdateBullets(vector<Bullet>& bullet_list, const Player& player) {
    const double RADIAL_SPEED = 0.0045;// 径向波动速度
    const double TANGENT_SPEED = 0.0055;// 切向波动速度
    double radianInterval = 2 * 3.14159 / bullet_list.size();// 子弹之间的弧度间隔
    double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
    for (int i = 0; i < bullet_list.size(); i++){
        double radian = GetTickCount() * TANGENT_SPEED + radianInterval * i;
        bullet_list[i].bullet_pos.x = player.GetPosition().x + player.PLAYER_WIDTH / 2 + (int)(radius * sin(radian));
        bullet_list[i].bullet_pos.y = player.GetPosition().y + player.PLAYER_WIDTH / 2 + (int)(radius * cos(radian));
    }
} 
int main()
{
    initgraph(1280, 720);// 创建绘图窗口

    bool running = true;

    ExMessage msg;// 消息结构体
    IMAGE img_background;// 背景图片
    Player player;// 玩家
    vector<Enemy*> enemy_list;// 敌人
    //Bullet bullet_list[3];// 子弹
    vector<Bullet> bullet_list(3);// 子弹

    loadimage(&img_background, _T("img/background.png"));

    BeginBatchDraw();// 开始批量绘图
    // 游戏主循环
    while (running)
    {
        DWORD start_time = GetTickCount();

        //1.获取输入
        while (peekmessage(&msg)){
            player.ProcessEvent(msg);
        }
        //2.数据处理
        player.Move();
        UpdateBullets(bullet_list, player);
        TryGenerateEnemy(enemy_list);
        for (Enemy* enemy : enemy_list)
            enemy->Move(player);


        for (Enemy* enemy : enemy_list) {
            // 检测敌人与玩家的碰撞
            if (enemy->CheckPlayerCollision(player)) {
                MessageBox(GetHWnd(), _T("死"), _T("游戏结束"), MB_OK);
                running = false;
                break;
            }
            // 检测敌人与子弹的碰撞
            for (const Bullet& bullet : bullet_list){
                if (enemy->CheckBulletCollision(bullet)){
                    enemy->Hurt();
                }
            }             
        }

        // 移除生命值为0的敌人
        for (int i = 0; i < enemy_list.size(); i++){
            Enemy* eny = enemy_list[i];
            if (!eny->CheckAlive()){
                swap(enemy_list[i], enemy_list.back());
                enemy_list.pop_back();
                delete eny;
            }
        } 

        //3.进行渲染
        cleardevice();
        putimage(0, 0, &img_background);
        player.Draw(1000 / 144);
        for (Enemy* enemy : enemy_list)
            enemy->Draw(1000 / 144);
        for (const Bullet& bullet : bullet_list)
            bullet.Draw(); 

        FlushBatchDraw();// 执行未完成的绘制任务

        DWORD end_time = GetTickCount();
        DWORD detle_time = end_time - start_time;
        if (detle_time < 1000 / 144)
            Sleep(1000 / 144 - detle_time);
    }
    EndBatchDraw();// 结束批量绘制并执行未完成的绘制任务
    return 0;
}

