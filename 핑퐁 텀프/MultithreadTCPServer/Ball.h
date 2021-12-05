#pragma once
#include "GameData.h"

class Ball
{
private:
	VECTOR2 m_vecPos;
	VECTOR2 m_vecForce;

public:
	void BallUpdate();
	VECTOR2 getBallPoint() { return m_vecPos; }

	//	ผผลอ
	void SetBallPoint(VECTOR2 _p) { m_vecPos = _p; }

public:

	Ball();
	~Ball();
};

