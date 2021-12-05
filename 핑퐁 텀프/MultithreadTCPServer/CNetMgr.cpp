#include "CNetMgr.h"

void CNetMgr::setPacketData(cs_packet_mainGame _pk)
{
	switch (_pk.pkType)
	{
	case PACKET_TYPE::NONE:
		
		break;
		
	case PACKET_TYPE::START:
		g_clientIDManager[g_uiIDCnt].uiID = g_uiIDCnt++;
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
