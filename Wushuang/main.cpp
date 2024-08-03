#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>

#include "Atlas.h"
#include "Animation.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Atlas* atlas_player_left;// 玩家向左动作帧序列
Atlas* atlas_player_right;// 玩家向右动作帧序列
Atlas* atlas_enemy_left;// 敌人向左动作帧序列
Atlas* atlas_enemy_right; // 敌人向右动作帧序列

// 生成新的敌人
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list) {
    const int INTERVAL = 100;
    static int counter = 0;
    if ((++counter) % INTERVAL == 0)
        enemy_list.push_back(new Enemy());
}

// 更新子弹位置
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player) {
    const double RADIAL_SPEED = 0.0045;// 径向波动速度
    const double TANGENT_SPEED = 0.0055;// 切向波动速度
    double radianInterval = 2 * 3.14159 / bullet_list.size();// 子弹之间的弧度间隔
    double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
    for (int i = 0; i < bullet_list.size(); i++) {
        double radian = GetTickCount() * TANGENT_SPEED + radianInterval * i;
        bullet_list[i].bullet_pos.x = player.GetPosition().x + player.PLAYER_WIDTH / 2 + (int)(radius * sin(radian));
        bullet_list[i].bullet_pos.y = player.GetPosition().y + player.PLAYER_WIDTH / 2 + (int)(radius * cos(radian));
    }
}

// 绘制玩家得分
void DrawPlayScore(int score) {
    static TCHAR text[64];
    _stprintf_s(text, _T("当前玩家得分：%d"), score);

    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 85, 185));
    outtextxy(10, 10, text);
}

int main()
{
    initgraph(1280, 720);// 创建绘图窗口

    atlas_player_left = new Atlas(_T("img/paimon_left_%d.png"), 6); // 动画由6张图片构成，
    atlas_player_right = new Atlas(_T("img/paimon_right_%d.png"), 6);
    atlas_enemy_left = new Atlas(_T("img/boar_left_%d.png"), 6);
    atlas_enemy_right = new Atlas(_T("img/boar_right_%d.png"), 6);

    mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);// 背景音乐
    mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);///攻击音乐

    mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);

    bool running = true;
    int score = 0;// 得分

    ExMessage msg;// 消息结构体
    IMAGE img_background;// 背景图片
    Player player;// 玩家
    std::vector<Enemy*> enemy_list;// 敌人
    //Bullet bullet_list[3];// 子弹
    std::vector<Bullet> bullet_list(3);// 子弹

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

        // 检测碰撞
        for (Enemy* enemy : enemy_list) {
            // 检测敌人与玩家的碰撞
            if (enemy->CheckPlayerCollision(player)) {
                static TCHAR text[128];
                _stprintf_s(text, _T("最终得分：%d！"), score);
                //MessageBox(GetHWnd(), _T("死"), _T("游戏结束"), MB_OK);
                MessageBox(GetHWnd(), text, _T("游戏结束"), MB_OK);
                running = false;
                break;
            }
            // 检测敌人与子弹的碰撞
            for (const Bullet& bullet : bullet_list){
                if (enemy->CheckBulletCollision(bullet)){
                    mciSendString(_T("play hit from 0"), NULL, 0, NULL);
                    enemy->Hurt();
                    score++;
                }
            }             
        }

        // 移除生命值为0的敌人
        for (int i = 0; i < enemy_list.size(); i++){
            Enemy* enemy = enemy_list[i];
            if (!enemy->CheckAlive()){
                std::swap(enemy_list[i], enemy_list.back());
                enemy_list.pop_back();
                delete enemy;
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
        DrawPlayScore(score);

        FlushBatchDraw();// 执行未完成的绘制任务

        DWORD end_time = GetTickCount();
        DWORD detle_time = end_time - start_time;
        if (detle_time < 1000 / 144)
            Sleep(1000 / 144 - detle_time);
    }

    delete atlas_player_left;
    delete atlas_player_right;
    delete atlas_enemy_left;
    delete atlas_enemy_right;

    EndBatchDraw();// 结束批量绘制并执行未完成的绘制任务
    return 0;
}

