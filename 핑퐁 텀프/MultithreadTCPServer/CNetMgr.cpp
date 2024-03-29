#include "CNetMgr.h"
#include "Collision.h"

int iUpdateCnt;

void CNetMgr::update()
{
	VECTOR2 temp{};
	temp.x = m_Ball.getBallPoint().x + ( m_Ball.getBallForce().x * 0.5f );
	temp.y = m_Ball.getBallPoint().y + ( m_Ball.getBallForce().y * 0.5f );
	m_Ball.SetBallPoint(temp);

	for (int i = 0; i < 2; ++i)		//	AABB - player bar x Ball
	{
		BOOL bTemp = Collision(m_vecData[i].m_vecPos, m_Ball.getBallPoint());	//	충돌 시 return true
		if (bTemp)
		{
			VECTOR2 changeForce = m_Ball.getBallForce();
			changeForce.x *= -1.25f;
			m_Ball.SetBallForce(changeForce);
			return;
		}
	}   

	if (m_Ball.getBallPoint().x < -10.f) 
	{	
		m_vecData[1].m_uiScore += 1;
		CheckGameStatus();
		//	플레이어 2 +1점
	}
	else if (m_Ball.getBallPoint().x > 1290.f)
	{
		m_vecData[0].m_uiScore += 1; 
		CheckGameStatus();
		//	플레이어 1 +1점
	}

	VECTOR2 vecForce = m_Ball.getBallForce();
	if (m_Ball.getBallPoint().y <= 0.f )		//	위 아래 공 반대로 튕기기
	{
		vecForce.y = fabs(vecForce.y);
		m_Ball.SetBallForce(vecForce);
	}

	if (m_Ball.getBallPoint().y >= 630.f)
	{
		vecForce.y = fabs(vecForce.y) * -1.f;
		m_Ball.SetBallForce(vecForce);
	}

	cout << iUpdateCnt++ << "번째 업데이트" << endl;
}

BOOL CNetMgr::Collision(VECTOR2 _playerPos, VECTOR2 _ballPos)
{
	COLLISON_PLAYER player{_playerPos.x + 10.f, _playerPos.y + 10.f, _playerPos.x + 40.f, _playerPos.y + 100.f };	//	빨강
	COLLISON_PLAYER	ball{ _ballPos.x - 10.f, _ballPos.y - 10.f, _ballPos.x + 10.f, _ballPos.y + 10.f };				//	파랑

	if (ball.Right > player.Left && player.Right > ball.Left && ball.Top < player.Bottom && player.Top < ball.Bottom)
		return true;

	return false;
}

void CNetMgr::CheckGameStatus()
{
	VECTOR2 resetPoint{ 640, 360 };
	VECTOR2 resetForce{ m_Ball.getBallForce().x, m_Ball.getBallForce().y };
	if (resetForce.x < 0.f)
		resetForce.x = -8.f;
	else
		resetForce.x = 8.f;

	if (resetForce.y < 0.f)
		resetForce.y = -4.f;
	else
		resetForce.y = 4.f;
	m_Ball.SetBallPoint(resetPoint);
	m_Ball.SetBallForce(resetForce);


	//ResetGame(bGameEnd, iWinnerID);
}

PACKET_TYPE CNetMgr::ResetGame(BOOL _bStatus, UINT _uiID) //리턴 타입 변경 (void -> PACKET_TYPE)
{
	if (!_bStatus)
		return PACKET_TYPE::MAIN;


	for (int i = 0; i < 2; ++i)
	{
		m_vecData[i].m_uiScore = 0;
	}
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
		ResetGame(true, 0);

		return PACKET_TYPE::MAIN;
		break;

	case PACKET_TYPE::MAIN:
		
		m_vecData[_pk.uiPlayerID].m_vecPos = _pk.ptPos;


		//for (int i = 0; i < 2; ++i) { // 3점 만족하면 END 리턴
		//	if (m_vecData[i].m_uiScore >= 3) {
		//		m_vecData[_pk.uiPlayerID].m_status = PACKET_TYPE::END;
		//		return PACKET_TYPE::END;
		//	}
		//}

		if (m_vecData[0].m_uiScore >= 3 || m_vecData[1].m_uiScore >= 3)
		{
			return PACKET_TYPE::END;

		}

		return PACKET_TYPE::MAIN;
		break;

	case PACKET_TYPE::END:

		g_bGameStart = false;
		return PACKET_TYPE::READY;

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
