#include "CNetMgr.h"

void CNetMgr::setPacketData(cs_packet_mainGame _pk)
{
	switch (_pk.pkType)
	{
	case PACKET_TYPE::NONE:
		
		break;
		
	case PACKET_TYPE::START:
		
		if (g_uiIDCnt == 1)
		{
			int a = 0;
		}
		if (g_uiIDCnt > 1)  //  ������ ������ Ŭ�� 1���� ������ ������ �����϶�
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
