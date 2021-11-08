#pragma once
struct VECTOR2
{
	float x;
	float y;
};

class Ball
{
private:
	POINT ptPoint;
	VECTOR2 vecForce;

public:

	Ball();
	~Ball();
};

