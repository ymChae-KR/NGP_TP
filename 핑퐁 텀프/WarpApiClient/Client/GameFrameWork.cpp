#include "stdafx.h"
#include "GameFramework.h"
#include "Client.h"


void WGameFramework::SetPlayerData(cs_packet_mainGame _packet)
{
	if (_packet.uiPlayerID == m_uiID)
	{
		//m_uiID = _packet.uiPlayerID;
		m_pPlayer->SetPlayerPoint(_packet.ptPos);
	}
}

void WGameFramework::SetClientID(UINT _ID)
{
	m_uiID = _ID;

	// 플레이어 ID값 별로 분기 해서 컨트롤할 플레이어 지정해야함
	if (m_uiID == 0)
	{
		m_pPlayer->SetPlayerPoint(VECTOR2(10, 10));
		m_pEnemy->SetPlayerPoint(VECTOR2(1200, 10));
	}
	else if (m_uiID == 1)
	{
		m_pPlayer->SetPlayerPoint(VECTOR2(1200, 10));
		m_pEnemy->SetPlayerPoint(VECTOR2(10, 10));
	}
	else
	{
		cout << "플레이어 ID 값 오류" << endl;
	}
}

void WGameFramework::SetPlayerPos(cs_packet_mainGame _packet)
{
	m_pEnemy->SetPlayerPoint(_packet.ptPos);

}

void WGameFramework::SetBallPos(cs_packet_mainGame _packet) {

	m_pBall->SetBallPoint(_packet.bPos);
}

void WGameFramework::SetEnemyData(cs_packet_mainGame _packet)
{
	if (_packet.uiPlayerID != m_uiID)
	{
		m_pEnemy->SetPlayerPoint(_packet.ptPos);
	}
}

WGameFramework::WGameFramework()
{
	Clear();
	m_pPlayer = new Player;
	m_pEnemy = new Player;
	m_pBall = new Ball;

	m_pPlayer->SetPlayerPoint(VECTOR2(0.f, 0.f));
	m_pEnemy->SetPlayerPoint(VECTOR2(0.f, 0.f));
	m_pBall->SetBallPoint(VECTOR2(0.f, 0.f));
}

WGameFramework::~WGameFramework()
{
}

void WGameFramework::Reset()
{
}

void WGameFramework::Clear()
{
}

void WGameFramework::Create(HWND hWnd)
{
	m_hWnd = hWnd;
}

void EllipseR(HDC hdc, int x, int y, int r) { //(x,y) 를 중심으로 하는 반지름 r의 공 그리기

	Ellipse(hdc, x - r, y - r, x + r, y + r);
}

void WGameFramework::OnDraw(HDC hdc)
{
	float ball_x = m_pBall->getBallPoint().x;
	float ball_y = m_pBall->getBallPoint().y;

	Rectangle(hdc, m_pPlayer->getPlayerPoint().x + 10, m_pPlayer->getPlayerPoint().y + 10,
		m_pPlayer->getPlayerPoint().x + 40, m_pPlayer->getPlayerPoint().y + 100);

	Rectangle(hdc, m_pEnemy->getPlayerPoint().x + 10, m_pEnemy->getPlayerPoint().y + 10,
		m_pEnemy->getPlayerPoint().x + 40, m_pEnemy->getPlayerPoint().y + 100);

	EllipseR(hdc, ball_x, ball_y, 10);
	ball_x += 10;
}

void WGameFramework::OnUpdate(const float frameTime)
{
	Interaction();
}

void WGameFramework::KeyBoard(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (wParam == VK_Q)
		{
			SendMessage(m_hWnd, WM_DESTROY, 0, 0);
			return;
		}

		if (GetAsyncKeyState(VK_UP))
			m_pPlayer->SetPlayerPoint(VECTOR2(m_pPlayer->getPlayerPoint().x, m_pPlayer->getPlayerPoint().y - 10.f));
		if (GetAsyncKeyState(VK_DOWN))
			m_pPlayer->SetPlayerPoint(VECTOR2(m_pPlayer->getPlayerPoint().x, m_pPlayer->getPlayerPoint().y + 10.f));

	}
	break;


	case WM_KEYUP:
	{

	}
	break;


	}

}