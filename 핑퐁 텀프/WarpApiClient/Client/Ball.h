#pragma once
class Ball
{
private:
	POINT ptPoint;
	VECTOR2 ballPoint;
	VECTOR2 vecForce;

	float radius;

public:

	Ball();
	~Ball();

	VECTOR2 getBallPoint() { return ballPoint; }

	//	����
	void SetBallPoint(VECTOR2 _p) { ballPoint = _p; }
	//void SetPlayerWidth(float _f) { radius = _f; }
};

