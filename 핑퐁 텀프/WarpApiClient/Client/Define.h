#pragma once
#define MAIN_TIMER			1	
#define MAIN_TIEMR_FRAME	17 // 60FPS

#define FRAME_WIDTH			1280
#define FRAME_HEIGHT		720

enum PACKET_TYPE
{
	NONE,
	START,
	READY,
	MAIN,
	END,
};

// KeyBoard
enum VK_KEY
{
	VK_0 = 48,
	VK_1 = 49,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,
	VK_A = 0x41,
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z
};

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#define global_variable static
#define internal static

inline int
clamp(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}

struct VECTOR2
{
	float x;
	float y;
};

#pragma pack (push, 1)

struct cs_packet_mainGame
{
	PACKET_TYPE pkType;
	VECTOR2 ptPos;
	VECTOR2 bPos;
	UINT uiPlayerID;
};

struct sc_packet_mainGame
{
	PACKET_TYPE pkType;
	VECTOR2		vec2Pos;
	VECTOR2		bPos;
	UINT		uiPlayerID;
	UINT		uiScore;
};

//struct Ball_Pos {
//	
//	VECTOR2 ballPos;
//	//UINT ballID;
//	//bool ballState;
//};
//

#pragma pack (pop)