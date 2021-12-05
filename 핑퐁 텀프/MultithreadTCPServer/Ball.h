#pragma once
#include "GameData.h"

class Ball
{
private:
	VECTOR2 m_vecPos;
	VECTOR2 m_vecForce;

public:
	void BallUpdate();

	//	Get
	VECTOR2 getBallPoint() { return m_vecPos; }
	VECTOR2 getBallForce() { return m_vecForce; }

	//	Set
	void SetBallPoint(VECTOR2 _p) { m_vecPos = _p; }
	void SetBallForce(VECTOR2 _p) { m_vecForce = _p; }

public:

	Ball();
	~Ball();
};

