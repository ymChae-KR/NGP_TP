typedef struct //�÷��̾� ����ü
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
	//����
	float cX, cY, cZ = 0.0f;

	//�÷��̾� ����
	float PL = p->Left,
		PR = p->Right,
		PT = p->Top,
		PB = p->Bottom;

	//���� ����
	float BL = b->fX - b->fR,
		BR = b->fX + b->fR,
		BT = b->fY + b->fR,
		BB = b->fY + b->fR;

	if (BL > PR) return false;
	if (BR < PL) return false;
	if (BB < PT) return false;
	if (BT > PB) return false;

	// ���� ������ �÷��̾� TOP~BOTTOM ����
	if (b->fY > PT && b->fY < PB) return true;

	// �ڽ��� LEFT~RIGHT ����
	if (b->fX > PL && b->fX < PR) return true;

	cZ = b->fR * b->fR; //������ ����

	//���� �𼭸�
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

	//������ �𼭸�
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