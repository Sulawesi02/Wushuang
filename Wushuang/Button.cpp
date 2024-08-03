#include "Button.h"

Button::Button(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed) {
	region = rect;
	loadimage(&img_idle, path_img_idle);
	loadimage(&img_hovered, path_img_hovered);
	loadimage(&img_pushed, path_img_pushed);
}

// 检测鼠标点击
bool Button::CheckCursorHit(int x, int y) {
	return x >= region.left && x <= region.right && y >= region.top && y <= region.bottom;
}

void Button::ProcessEvent(const ExMessage& msg) {
	switch (msg.message) {
	case WM_MOUSEMOVE:
		// 如果按钮处于默认状态并且鼠标移动到按钮上时，转悬停状态
		if (status == Status::Idle && CheckCursorHit(msg.x, msg.y))
			status = Status::Hovered;
		// 如果鼠标处于悬停状态时并且鼠标移出按钮时，转默认状态
		else if (status == Status::Hovered && !CheckCursorHit(msg.x, msg.y))
			status = Status::Idle;
		break;
	case WM_LBUTTONDOWN:
		//如果鼠标处于左键按下状态时，转按压状态
		if (CheckCursorHit(msg.x, msg.y))
			status = Status::Pushed;
		break;
	case WM_LBUTTONUP:
		// 如果按压回弹，完成一次点击动作
		if (status == Status::Pushed) {
			OnClick();
			status = Status::Idle;
		}
		break;
	default:
		break;
	}
}

void Button::Draw() {
	switch (status) {
	case Status::Idle:
		putimage(region.left, region.top, &img_idle);
		break;
	case Status::Hovered:
		putimage(region.left, region.top, &img_hovered);
		break;
	case Status::Pushed:
		putimage(region.left, region.top, &img_pushed);
		break;
	}
}

StartGameButton::StartGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
	: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}

void StartGameButton::OnClick() {
	is_game_started = true;
	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
}

QuitGameButton::QuitGameButton(RECT rect, LPCTSTR path_img_idle, LPCTSTR path_img_hovered, LPCTSTR path_img_pushed)
	: Button(rect, path_img_idle, path_img_hovered, path_img_pushed) {}

void QuitGameButton::OnClick() {
	is_game_running = false;
}
