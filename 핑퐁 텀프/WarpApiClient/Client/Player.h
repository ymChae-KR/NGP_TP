#pragma once
class Player
{
private:
	VECTOR2 ptPoint;	//	중점

	float fWidth;
	float fHegiht;

public:
	Player();
	~Player();

	//	게터
	VECTOR2 getPlayerPoint() { return ptPoint; }

	//	세터
	void SetPlayerPoint(VECTOR2 _p) { ptPoint = _p; }
	void SetPlayerWidth(float _f) { fWidth = _f; }
	void SetPlayerHeight(float _f) { fHegiht = _f; }


};

