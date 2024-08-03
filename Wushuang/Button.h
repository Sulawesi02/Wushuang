#ifndef BUTTON_H
#define BUTTON_H

#include<graphics.h>
#include<string>
#include<vector>
#include<windows.h>

extern bool is_game_started;
extern bool is_game_running;

class Button{
	enum class Status{
		Idle = 0,
		Hovered,
		Pushed
	};

	RECT region;// ��Χ
	IMAGE img_idle;// Ĭ��
	IMAGE img_hovered;// ��ͣ
	IMAGE img_pushed;// ��ѹ
	Status status = Status::Idle;

protected:
	virtual void OnClick() = 0;

public:
	Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~Button() = default;

	bool CheckCursorHit(int x, int y);
	void ProcessEvent(const ExMessage& msg);
	void Draw();
};

// ��ʼ��Ϸ��ť
class StartGameButton : public Button{

public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~StartGameButton() = default;

protected:
	void OnClick() override;
};

//�˳���Ϸ��ť
class QuitGameButton : public Button{

public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~QuitGameButton() = default;

protected:
	void OnClick() override;
}; 

#endif 