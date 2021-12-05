#include "Ball.h"

Ball::Ball()
{
	//	공 포지션
	m_vecPos.x = 600.f;
	m_vecPos.y = 400.f;

	//	공 방향 벡터 초기화
	m_vecForce.x = -4.f;
	m_vecForce.y = -1.5f;

}

Ball::~Ball()
{
}
