#include "CNetMgr.h"

CNetMgr::CNetMgr()
{
	m_vecData.resize(2);
}

CNetMgr::~CNetMgr()
{
}

//gameData CNetMgr::getOtherPlayerData(UINT _id)
//{
//	UINT PID{};
//
//	if (_id == 0)
//		PID = 1;
//	else if (_id == 1)
//		PID = 0;
//	else
//		PID == 2;
//
//	return m_vecData[PID];
//}
