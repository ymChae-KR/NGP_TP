#pragma once
class Player
{
private:
	VECTOR2 ptPoint;	//	����

	float fWidth;
	float fHegiht;

public:
	Player();
	~Player();

	//	����
	VECTOR2 getPlayerPoint() { return ptPoint; }

	//	����
	void SetPlayerPoint(VECTOR2 _p) { ptPoint = _p; }
	void SetPlayerWidth(float _f) { fWidth = _f; }
	void SetPlayerHeight(float _f) { fHegiht = _f; }


};

