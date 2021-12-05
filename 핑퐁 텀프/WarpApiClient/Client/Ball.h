#pragma once


class Ball
{
private:
	int isReady;
	POINT ptPoint;
	VECTOR2 ballPoint;
	VECTOR2 vecForce;
	clock_t moveTime;
	clock_t lastTime;

	//float radius;

public:
	void BallUpdate();

public:

	Ball();
	~Ball();

	VECTOR2 getBallPoint() { return ballPoint; }

	//	ผผลอ
	void SetBallPoint(VECTOR2 _p) { ballPoint = _p; }
	//void SetPlayerWidth(float _f) { radius = _f; }
};

