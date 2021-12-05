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
<<<<<<< HEAD
	PACKET_TYPE		 pkType;
	unsigned int	 uiPlayerID;
	VECTOR2			 ptPos;
=======
	PACKET_TYPE pkType;
	VECTOR2 ptPos;
	VECTOR2 bPos;
	UINT uiPlayerID;
>>>>>>> 40428dc8dcabb4c51a434c4b4f0e03ae7e5e56be
};

struct sc_packet_mainGame
{
<<<<<<< HEAD
	PACKET_TYPE			pkType;
	unsigned int		uiPlayerID;
	VECTOR2				vec2Pos;
=======
	PACKET_TYPE pkType;
	VECTOR2		vec2Pos;
	VECTOR2		bPos;
	UINT		uiPlayerID;
>>>>>>> 40428dc8dcabb4c51a434c4b4f0e03ae7e5e56be
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
