#pragma once
#include "ServerData.h"
#include "GameData.h"
#include "Ball.h"
#include <vector>

struct gameData
{
	UINT		m_ID;		//	각 플레이어 ID
	VECTOR2		m_vecPos;	//	플레이어 Pos
	PACKET_TYPE	m_status{ PACKET_TYPE::NONE };	//	클라 별 게임 상태 저장 변수
};

//	네트워크 매니저는 각 쓰레드에서 받은 패킷 데이터를 바탕으로 전체 게임을 컨트롤한다.
//	공유자원에 대한 해결책이 필요함
class CNetMgr
{
private:
	std::vector<gameData>	 m_vecData;		//	플레이어 객체 관리할 컨테이너
	Ball					 m_Ball;

public:
	void update();
	BOOL Collision(VECTOR2 _playerPos, VECTOR2 _ballPos);

public:
	//	getter
	std::vector<gameData> getData() { return m_vecData; }
	gameData getOtherPlayerData(UINT _id) { return m_vecData[_id]; }
	gameData getBallData(UINT _id) { return m_vecData[_id]; }

	Ball	 getBall() { return m_Ball; }

	//	setter
	void setData(gameData _g) { m_vecData.push_back(_g); }
	PACKET_TYPE setPacketData(cs_packet_mainGame _pk);

public:
	CNetMgr();
	~CNetMgr();

};
