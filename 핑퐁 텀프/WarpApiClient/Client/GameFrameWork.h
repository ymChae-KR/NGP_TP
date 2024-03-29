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
	UINT	m_uiScore;
	UINT	m_emScore; //상대 점수 추가

public:
	//	getter
	UINT	GetID() { return m_uiID; }
	VECTOR2 GetPlayerPos() { return m_pPlayer->getPlayerPoint(); }
	VECTOR2 GetBallPos() { return m_pBall->getBallPoint(); }
	UINT	GetScore() { return m_uiScore; }
	
	//	setter
	void SetClientID(UINT _ID);
	void SetPlayerData(cs_packet_mainGame _packet);
	void SetPlayerPos(sc_packet_mainGame _packet);
	void SetBallPos(sc_packet_mainGame _packet);
	void SetEnemyData(sc_packet_mainGame _packet);
	void SetScore(sc_packet_mainGame _packet);
	
public:
	WGameFramework();
	~WGameFramework();

	virtual void Reset();
	virtual void Clear();

	virtual void Create(HWND hWnd);
	virtual void OnDraw(HDC hdc);
	virtual void DrawScore(HDC hdc); // 추가
	virtual void OnUpdate(const float frameTime = 0.17f);
	virtual void KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam);

private:
	HWND	m_hWnd;
};
