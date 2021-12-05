#include "CNetMgr.h"

void CNetMgr::setPacketData(cs_packet_mainGame _pk)
{
	if (!(_pk.uiPlayerID == 0 || _pk.uiPlayerID == 1 || _pk.uiPlayerID == 2))
	{
		cout << "쓰레기 패킷 recv" << endl;
		return;
	}

	switch (_pk.pkType)
	{
	case PACKET_TYPE::NONE:
		
		break;
		
	case PACKET_TYPE::START:
		
		if (g_uiIDCnt == 1)
		{
			int a = 0;
		}
		if (g_uiIDCnt > 1)  //  서버에 접속한 클라가 1명보다 많으면 게임을 시작하라
			g_bGameStart = true;
		break;

	case PACKET_TYPE::MAIN:
		m_vecData[_pk.uiPlayerID].m_vecPos = _pk.ptPos;
		break;
	}
}

CNetMgr::CNetMgr()
{
	m_vecData.resize(2);
}

CNetMgr::~CNetMgr()
{
}
