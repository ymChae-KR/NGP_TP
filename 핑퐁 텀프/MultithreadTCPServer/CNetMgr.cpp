#include "CNetMgr.h"

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
		m_vecData[_pk.uiPlayerID].m_ballPos = _pk.bPos;

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
