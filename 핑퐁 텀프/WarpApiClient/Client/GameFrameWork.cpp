#include "stdafx.h"
#include "GameFramework.h"
#include "Client.h"


void WGameFramework::SetPlayerData(cs_packet_mainGame _packet)
{
	m_uiID = _packet.uiPlayerID;
	m_pPlayer->SetPlayerPoint(_packet.ptPos);
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

WGameFramework::WGameFramework()
{
	Clear();
	m_pPlayer = new Player;
	m_pEnemy = new Player;

	m_pPlayer->SetPlayerPoint(VECTOR2(0.f, 0.f));
	m_pEnemy->SetPlayerPoint(VECTOR2(0.f, 0.f));
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
	 
	//	서버에서 클라 아이디 수신 후 PID 초기화
	//cs_packet_mainGame* data = reinterpret_cast<cs_packet_mainGame*>(Recv_Packet());
	//m_uiID = data->uiPlayerID;
}

void WGameFramework::OnDraw(HDC hdc)
{
	Rectangle(hdc, m_pPlayer->getPlayerPoint().x + 10, m_pPlayer->getPlayerPoint().y + 10,
		m_pPlayer->getPlayerPoint().x + 40, m_pPlayer->getPlayerPoint().y + 100);

	Rectangle(hdc, m_pEnemy->getPlayerPoint().x + 10, m_pEnemy->getPlayerPoint().y + 10,
		m_pEnemy->getPlayerPoint().x + 40, m_pEnemy->getPlayerPoint().y + 100);

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
				m_pPlayer->SetPlayerPoint(VECTOR2(m_pPlayer->getPlayerPoint().x ,m_pPlayer->getPlayerPoint().y - 10.f));
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
