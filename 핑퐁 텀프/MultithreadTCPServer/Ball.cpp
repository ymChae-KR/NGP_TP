#include "Ball.h"

Ball::Ball()
{
	//	�� ������
	m_vecPos.x = 600.f;
	m_vecPos.y = 400.f;

	//	�� ���� ���� �ʱ�ȭ
	m_vecForce.x = -4.f;
	m_vecForce.y = -1.5f;

}

Ball::~Ball()
{
}
