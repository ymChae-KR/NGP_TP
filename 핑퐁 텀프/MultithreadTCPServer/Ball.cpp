#include "Ball.h"

Ball::Ball()
{
	//	공 포지션
	m_vecPos.x = 600.f;
	m_vecPos.y = 400.f;

	//	공 방향 벡터 초기화
	m_vecForce.x = -8.f;
	m_vecForce.y = -2.f;

}

Ball::~Ball()
{
}
