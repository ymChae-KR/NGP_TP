#pragma once
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


struct VECTOR2
{
	float x;
	float y;

	void Set(float _x, float _y) { x = _x; y = _y; }
};

enum PACKET_TYPE
{
	NONE,
	START,
	READY,
	MAIN,

};


struct ID
{
	SOCKADDR_IN				sc_Client_Address;
	unsigned int			uiID;
	unsigned int			uiScore;
};

struct cs_packet_mainGame
{

	PACKET_TYPE pkType;
	VECTOR2 ptPos;
	VECTOR2 bPos;
	UINT uiPlayerID;
};

struct sc_packet_mainGame
{
	PACKET_TYPE pkType;
	VECTOR2		vec2Pos;
	VECTOR2		bPos;
	UINT		uiPlayerID;
	UINT		uiScore;
};