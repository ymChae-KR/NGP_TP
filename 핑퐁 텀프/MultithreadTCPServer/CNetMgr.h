#pragma once
#include "ServerData.h"
#include "GameData.h"
#include <vector>

struct gameData
{
	UINT		m_ID;		//	각 플레이어 ID
	VECTOR2		m_vecPos;	//	플레이어 Pos

};

//	네트워크 매니저는 각 쓰레드에서 받은 패킷 데이터를 바탕으로 전체 게임을 컨트롤한다.
//	공유자원에 대한 해결책이 필요함
class CNetMgr
{
private:
	vector<gameData> m_vecData;
	
public:

	//	getter
	std::vector<gameData> getData() { return m_vecData; }
	gameData getOtherPlayerData(UINT _id) { return m_vecData[_id]; }
	
	//	setter
	void setData(gameData _g) { m_vecData.push_back(_g); }
	void setPacketData(cs_packet_mainGame _pk) { m_vecData[_pk.uiPlayerID].m_vecPos = _pk.ptPos; }

public:
	CNetMgr();
	~CNetMgr();

};

