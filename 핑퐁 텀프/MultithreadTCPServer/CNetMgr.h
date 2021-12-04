#pragma once
#include "ServerData.h"
#include "GameData.h"
#include <vector>

struct gameData
{
	UINT		m_ID;		//	�� �÷��̾� ID
	VECTOR2		m_vecPos;	//	�÷��̾� Pos

};

//	��Ʈ��ũ �Ŵ����� �� �����忡�� ���� ��Ŷ �����͸� �������� ��ü ������ ��Ʈ���Ѵ�.
//	�����ڿ��� ���� �ذ�å�� �ʿ���
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

