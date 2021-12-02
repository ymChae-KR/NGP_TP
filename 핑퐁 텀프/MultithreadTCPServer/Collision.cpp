typedef struct //플레이어 구조체
{
	float Left;
	float Top;
	float Right;
	float Bottom;
}PLAYER;

typedef struct
{
	float fX;
	float fY;
	float fR;

}BALL;

bool CollisionCheck(PLAYER* p, BALL* b)
{
	//제곱
	float cX, cY, cZ = 0.0f;

	//플레이어 범위
	float PL = p->Left,
		PR = p->Right,
		PT = p->Top,
		PB = p->Bottom;

	//공의 범위
	float BL = b->fX - b->fR,
		BR = b->fX + b->fR,
		BT = b->fY + b->fR,
		BB = b->fY + b->fR;

	if (BL > PR) return false;
	if (BR < PL) return false;
	if (BB < PT) return false;
	if (BT > PB) return false;

	// 원의 중점이 플레이어 TOP~BOTTOM 사이
	if (b->fY > PT && b->fY < PB) return true;

	// 박스의 LEFT~RIGHT 사이
	if (b->fX > PL && b->fX < PR) return true;

	cZ = b->fR * b->fR; //반지름 제곱

	//왼쪽 모서리
	cX = (PL - b->fX);
	cX *= cX;

	//TOP
	cY = (PB - b->fY);
	cY *= cY;
	if (cX + cY <= cZ) return true;

	//BOTTOM
	cY = (PB - b->fY);
	cY *= cY;
	if (cX + cY <= cZ) return true;

	//오른쪽 모서리
	cX = (PR - b->fX);
	cX *= cX;

	//TOP
	cY = (PT - b->fY);
	cY *= cY;
	if (cX + cY <= cZ) return true;

	//BOTTOM
	cY = (PB - b->fY);
	cY *= cY;
	if (cX + cY <= cZ) return true;

	return false;

}