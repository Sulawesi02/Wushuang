#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>

#include "Atlas.h"
#include "Animation.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Button.h"

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int BUTTON_WIDTH = 192;
const int BUTTON_HEIGHT = 75;

bool is_game_started = false;
bool is_game_running = true;

Atlas* atlas_player_left;// ���������֡����
Atlas* atlas_player_right;// ������Ҷ���֡����
Atlas* atlas_enemy_left;// ����������֡����
Atlas* atlas_enemy_right; // �������Ҷ���֡����

// �����µĵ���
void TryGenerateEnemy(std::vector<Enemy*>& enemy_list) {
    const int INTERVAL = 100;
    static int counter = 0;
    if ((++counter) % INTERVAL == 0)
        enemy_list.push_back(new Enemy());
}

// �����ӵ�λ��
void UpdateBullets(std::vector<Bullet>& bullet_list, const Player& player) {
    const double RADIAL_SPEED = 0.0045;// ���򲨶��ٶ�
    const double TANGENT_SPEED = 0.0055;// ���򲨶��ٶ�
    double radianInterval = 2 * 3.14159 / bullet_list.size();// �ӵ�֮��Ļ��ȼ��
    double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
    for (int i = 0; i < bullet_list.size(); i++) {
        double radian = GetTickCount() * TANGENT_SPEED + radianInterval * i;
        bullet_list[i].bullet_pos.x = player.GetPosition().x + player.PLAYER_WIDTH / 2 + (int)(radius * sin(radian));
        bullet_list[i].bullet_pos.y = player.GetPosition().y + player.PLAYER_WIDTH / 2 + (int)(radius * cos(radian));
    }
}

// ������ҵ÷�
void DrawPlayScore(int score) {
    static TCHAR text[64];
    _stprintf_s(text, _T("��ǰ��ҵ÷֣�%d"), score);

    setbkmode(TRANSPARENT);
    settextcolor(RGB(255, 85, 185));
    outtextxy(10, 10, text);
}

int main()
{
    initgraph(1280, 720);// ������ͼ����

    atlas_player_left = new Atlas(_T("img/paimon_left_%d.png"), 6); // ������6��ͼƬ���ɣ�
    atlas_player_right = new Atlas(_T("img/paimon_right_%d.png"), 6);
    atlas_enemy_left = new Atlas(_T("img/boar_left_%d.png"), 6);
    atlas_enemy_right = new Atlas(_T("img/boar_right_%d.png"), 6);

    mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);// ��������
    mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);///��������

    int score = 0;// �÷�
    ExMessage msg;// ��Ϣ�ṹ��
    IMAGE img_menu;// ���˵�
    IMAGE img_background;// ����ͼƬ
    Player player;// ���
    std::vector<Enemy*> enemy_list;// ����
    std::vector<Bullet> bullet_list(3);// �ӵ�

    RECT region_btn_start_game, region_btn_quit_game;

    region_btn_start_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    region_btn_start_game.right = region_btn_start_game.left + BUTTON_WIDTH;
    region_btn_start_game.top = 430;
    region_btn_start_game.bottom = region_btn_start_game.top + BUTTON_HEIGHT;


    region_btn_quit_game.left = (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    region_btn_quit_game.right = region_btn_quit_game.left + BUTTON_WIDTH;
    region_btn_quit_game.top = 550;
    region_btn_quit_game.bottom = region_btn_quit_game.top + BUTTON_HEIGHT; 

    StartGameButton btn_start_game = StartGameButton(region_btn_start_game, _T("img/ui_start_idle.png"), _T("img/ui_start_hovered.png"), _T("img/ui_start_pushed.png"));
    QuitGameButton btn_quit_game = QuitGameButton(region_btn_quit_game, _T("img/ui_quit_idle.png"), _T("img/ui_quit_hovered.png"), _T("img/ui_quit_pushed.png"));

    loadimage(&img_menu, _T("img/menu.png"));
    loadimage(&img_background, _T("img/background.png"));

    BeginBatchDraw();// ��ʼ������ͼ
    // ��Ϸ��ѭ��
    while (is_game_running)
    {
        DWORD start_time = GetTickCount();

        //1.��ȡ����
        while (peekmessage(&msg)){
            if (is_game_started){
                player.ProcessEvent(msg);
            }
            else{
                btn_start_game.ProcessEvent(msg);
                btn_quit_game.ProcessEvent(msg);
            } 
        }
        //2.��������
        if (is_game_started) {
            TryGenerateEnemy(enemy_list);// �����µĵ���
            player.Move();// �ƶ����
            UpdateBullets(bullet_list, player);// �����ӵ�λ��
            for (Enemy* enemy : enemy_list)//���µ���λ��
                enemy->Move(player);

            // �����ײ
            for (Enemy* enemy : enemy_list) {
                // ����������ҵ���ײ
                if (enemy->CheckPlayerCollision(player)) {
                    static TCHAR text[128];
                    _stprintf_s(text, _T("���յ÷֣�%d��"), score);
                    //MessageBox(GetHWnd(), _T("��"), _T("��Ϸ����"), MB_OK);
                    MessageBox(GetHWnd(), text, _T("��Ϸ����"), MB_OK);
                    is_game_running = false;
                    break;
                }
                // ���������ӵ�����ײ
                for (const Bullet& bullet : bullet_list) {
                    if (enemy->CheckBulletCollision(bullet)) {
                        mciSendString(_T("play hit from 0"), NULL, 0, NULL);
                        enemy->Hurt();
                        score++;
                    }
                }
            }

            // �Ƴ�����ֵΪ0�ĵ���
            for (int i = 0; i < enemy_list.size(); i++) {
                Enemy* enemy = enemy_list[i];
                if (!enemy->CheckAlive()) {
                    std::swap(enemy_list[i], enemy_list.back());
                    enemy_list.pop_back();
                    delete enemy;
                }
            }
        }
        //3.���ƻ���
        cleardevice();
        if (is_game_started) {
            putimage(0, 0, &img_background);
            player.Draw(1000 / 144);
            for (Enemy* enemy : enemy_list)
                enemy->Draw(1000 / 144);
            for (const Bullet& bullet : bullet_list)
                bullet.Draw();
            DrawPlayScore(score);
        }
        else {
            putimage(0, 0, &img_menu);
            btn_start_game.Draw();
            btn_quit_game.Draw();
        }
        FlushBatchDraw();// ִ��δ��ɵĻ�������

        DWORD end_time = GetTickCount();
        DWORD detle_time = end_time - start_time;
        if (detle_time < 1000 / 144)
            Sleep(1000 / 144 - detle_time);
    }

    delete atlas_player_left;
    delete atlas_player_right;
    delete atlas_enemy_left;
    delete atlas_enemy_right;

    EndBatchDraw();// �����������Ʋ�ִ��δ��ɵĻ�������
    return 0;
}

