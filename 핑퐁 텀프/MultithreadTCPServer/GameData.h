#pragma once
enum PACKET_TYPE
{
	NONE,
	START,
	MAIN,

};

struct VECTOR2
{
	float x;
	float y;
};

struct ID
{
	SOCKADDR_IN				sc_Client_Address;
	unsigned int			uiID;
};

struct cs_packet_mainGame
{
	PACKET_TYPE		 pkType;
	unsigned int	 uiPlayerID;
	VECTOR2			 ptPos;
};

struct sc_packet_mainGame
{
	PACKET_TYPE			pkType;
	unsigned int		uiPlayerID;
	VECTOR2				vec2Pos;
};

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
