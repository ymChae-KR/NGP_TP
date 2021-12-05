#include "CNetMgr.h"
#include "Collision.h"

void CNetMgr::update()
{
	VECTOR2 temp{};
	temp.x = m_Ball.getBallPoint().x + ( m_Ball.getBallForce().x * 0.5f );
	temp.y = m_Ball.getBallPoint().y + ( m_Ball.getBallForce().y * 0.5f );
	m_Ball.SetBallPoint(temp);

	for (int i = 0; i < 2; ++i)
	{
		BOOL bTemp = Collision(m_vecData[i].m_vecPos, m_Ball.getBallPoint());	//	충돌 시 return true
		if (bTemp)
		{
			system("cls"); 
			cout << "충돌 ! " << endl;
		}
		
	}
}

BOOL CNetMgr::Collision(VECTOR2 _playerPos, VECTOR2 _ballPos)
{
	COLLISON_PLAYER player{_playerPos.x + 10.f, _playerPos.y + 10.f, _playerPos.x + 40.f, _playerPos.y + 100.f };	//	빨강
	COLLISON_PLAYER	ball{ _ballPos.x + 10.f, _ballPos.y + 10.f, _ballPos.x + 10.f, _ballPos.y + 10.f };				//	파랑

	if (ball.Right > player.Left && player.Right > ball.Left && ball.Top < player.Bottom && player.Top < ball.Bottom)
		return true;

	return false;
}

PACKET_TYPE CNetMgr::setPacketData(cs_packet_mainGame _pk)
{
	if (!(_pk.uiPlayerID == 0 || _pk.uiPlayerID == 1 || _pk.uiPlayerID == 2))
	{
		cout << "쓰레기 패킷 recv" << endl;
		return PACKET_TYPE::NONE;
	}

	switch (_pk.pkType)
	{
	case PACKET_TYPE::NONE:
		break;
		
	case PACKET_TYPE::START:

		m_vecData[_pk.uiPlayerID].m_status = PACKET_TYPE::READY;
		m_vecData[_pk.uiPlayerID].m_vecPos = _pk.ptPos;
		//m_vecData[_pk.uiPlayerID].m_ballPos = _pk.bPos;

		return PACKET_TYPE::READY;
		break;

	case PACKET_TYPE::READY:

		for (int i = 0; i < 2; ++i)
		{
			if (m_vecData[i].m_status != PACKET_TYPE::READY)
			{
				g_bGameStart = false;
				return PACKET_TYPE::READY;
			}
		}

		g_bGameStart = true;

		return PACKET_TYPE::MAIN;
		break;

	case PACKET_TYPE::MAIN:

		m_vecData[_pk.uiPlayerID].m_vecPos = _pk.ptPos;
		//m_vecData[_pk.uiPlayerID].m_ballPos = _pk.bPos;

		return PACKET_TYPE::MAIN;
		break;
	}

	return PACKET_TYPE::NONE;
}

CNetMgr::CNetMgr()
{
	m_vecData.resize(2);
}

CNetMgr::~CNetMgr()
{
}
