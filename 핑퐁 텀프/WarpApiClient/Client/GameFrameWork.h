#pragma once
#include "Player.h"
#include "Ball.h"
#include "Client.h"

class WGameFramework
{
	UINT	m_uiID;
	Player* m_pPlayer;
	Player* m_pEnemy;
	Ball*	m_pBall;

public:
	//	getter
	UINT	GetID() { return m_uiID; }
	VECTOR2 GetPlayerPos() { return m_pPlayer->getPlayerPoint(); }
	VECTOR2 GetBallPos() { return m_pBall->getBallPoint(); }
	
	//	setter
	void SetClientID(UINT _ID);
	void SetPlayerData(cs_packet_mainGame _packet);
	void SetPlayerPos(cs_packet_mainGame _packet);
	void SetBallPos(cs_packet_mainGame _packet);
	void SetEnemyData(cs_packet_mainGame _packet);

	
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
