#include "Ball.h"

Ball::Ball()
{
	//	�� ������
	m_vecPos.x = 600.f;
	m_vecPos.y = 400.f;

	//	�� ���� ���� �ʱ�ȭ
	m_vecForce.x = -2.f;
	m_vecForce.y = -1.f;

}

Ball::~Ball()
{
}
