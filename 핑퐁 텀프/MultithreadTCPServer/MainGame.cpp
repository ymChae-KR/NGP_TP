#include "GameData.h"
#include <wingdi.h>
#include <Windows.h>
#include "draw.cpp"

float player_1_p, player_1_dp, player_2_p, player_2_dp;
float arena_half_size_x = 85, arena_half_size_y = 45;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_px, ball_py, ball_dx = 100, ball_dy, radius = 3;

int player_1_score, player_2_score = 0;

void simulate_player(float* p, float* dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + player_half_size_y > arena_half_size_y) {
		*p = arena_half_size_y - player_half_size_y;
		*dp = 0;
	}
	else if (*p - player_half_size_y < -arena_half_size_y) {
		*p = -arena_half_size_y + player_half_size_y;
		*dp = 0;
	}
}

bool aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y)
{
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

void ballcheck(float dt) {

	float player_1_ddp = 1000;

	float player_2_ddp = 0.f;

	simulate_player(&player_1_p, &player_1_dp, player_1_ddp, dt);
	simulate_player(&player_2_p, &player_2_dp, player_2_ddp, dt);

	ball_px += ball_dx * dt;
	ball_py += ball_dy * dt;

	if (aabb_vs_aabb(ball_px, ball_py, radius, radius, 80, player_1_p, player_half_size_x, player_half_size_y)) {
		ball_px = 80 - player_half_size_x - radius;
		ball_dx *= -1;
		ball_dy = (ball_py - player_1_p) * 2 + player_1_dp * .75f;
	}
	else if (aabb_vs_aabb(ball_px, ball_py, radius, radius, -80, player_2_p, player_half_size_x, player_half_size_y)) {
		ball_px = -80 + player_half_size_x + radius;
		ball_dx *= -1;
		ball_dy = (ball_py - player_2_p) * 2 + player_2_dp * .75f;
	}

	if (ball_py + radius > arena_half_size_y) {
		ball_py = arena_half_size_y - radius;
		ball_dy *= -1;
	}
	else if (ball_py - radius < -arena_half_size_y) {
		ball_py = -arena_half_size_y + radius;
		ball_dy *= -1;
	}

	if (ball_px + radius > arena_half_size_x) {
		ball_dx *= -1;
		ball_dy = 0;
		ball_px = 0;
		ball_py = 0;
		player_1_score++;
		//왼쪽 플레이어 득점
	}
	else if (ball_px - radius < -arena_half_size_x) {
		ball_dx *= -1;
		ball_dy = 0;
		ball_px = 0;
		ball_py = 0;
		player_2_score++;
		//오른쪽 플레이어 득점
	}

	printf("플레이어1 : %d / 플레이어2 : %d\n", player_1_score, player_2_score);

}
