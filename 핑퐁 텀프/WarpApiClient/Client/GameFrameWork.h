#pragma once
#include "Player.h"
#include "Ball.h"

class WGameFramework
{
	UINT	m_uiID;
	Player* m_pPlayer;
	Player* m_pEnemy;
	Ball*	m_pBall;
	
public:
	WGameFramework();
	~WGameFramework();

	virtual void Reset();
	virtual void Clear();

	virtual void Create(HWND hWnd);
	virtual void OnDraw(HDC hdc);
	virtual void OnUpdate(const float frameTime = 0.17f);
	virtual void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);

private:
	HWND	m_hWnd;
};

struct Render_State 
{
	int height, width;
	void* memory;

	BITMAPINFO bitmap_info;
};
