#pragma once
class Player
{
private:
	POINT ptPoint;	//	����

	float fWidth;
	float fHegiht;

public:
	Player();
	~Player();

	//	����
	POINT getPlayerPoint() { return ptPoint; }

	//	����
	void SetPlayerPoint(POINT _p) { ptPoint = _p; }
	void SetPlayerWidth(float _f) { fWidth = _f; }
	void SetPlayerHeight(float _f) { fHegiht = _f; }


};

