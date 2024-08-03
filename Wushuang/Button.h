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

	RECT region;// ·¶Î§
	IMAGE img_idle;// Ä¬ÈÏ
	IMAGE img_hovered;// ÐüÍ£
	IMAGE img_pushed;// °´Ñ¹
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

// ¿ªÊ¼ÓÎÏ·°´Å¥
class StartGameButton : public Button{

public:
	StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~StartGameButton() = default;

protected:
	void OnClick() override;
};

//ÍË³öÓÎÏ·°´Å¥
class QuitGameButton : public Button{

public:
	QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed);
	~QuitGameButton() = default;

protected:
	void OnClick() override;
}; 

#endif 