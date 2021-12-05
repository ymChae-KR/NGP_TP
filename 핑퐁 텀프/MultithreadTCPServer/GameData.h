#pragma once
enum PACKET_TYPE
{
	NONE,
	START,
	READY,
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
};

struct sc_packet_EndGame 
{
	bool bResult;
};

struct Ball_Pos {

	VECTOR2 ballPos;
	//UINT ballID;
	//bool ballState;
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
