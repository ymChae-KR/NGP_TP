#pragma once
class Player
{
private:
	POINT ptPoint;	//	중점

	float fWidth;
	float fHegiht;

public:
	Player();
	~Player();

	//	게터
	POINT getPlayerPoint() { return ptPoint; }

	//	세터
	void SetPlayerPoint(POINT _p) { ptPoint = _p; }
	void SetPlayerWidth(float _f) { fWidth = _f; }
	void SetPlayerHeight(float _f) { fHegiht = _f; }


};

