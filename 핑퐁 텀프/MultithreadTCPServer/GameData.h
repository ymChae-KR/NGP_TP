#pragma once
#include "ServerData.h"

struct sc_packet_mainGame {
	Point2d p2dPosition;
	int iSpeed;
	float2 fVector;
};

struct sc_packet_EndGame {
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