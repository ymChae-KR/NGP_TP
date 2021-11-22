#pragma once
#include "ServerData.h"

struct VECTOR2
{
	float x;
	float y;
};

struct ID
{
	SOCKADDR_IN		sc_Client_Address;
	UINT			uiID;
};

struct sc_packet_mainGame
{
	VECTOR2		vec2Pos;
	UINT		uiPlayerID;
};	//	�÷��̾ ����ȭ ���� �Ϸ� �� ��Ŷ ������ ����

struct sc_packet_EndGame 
{
	bool bResult;
};

class MainGame
{
public:
	MainGame();
	~MainGame();

public:
	void BallUpdate();
	void playerUpdate();
	void notifyCollisions();
};
