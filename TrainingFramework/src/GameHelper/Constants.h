#pragma once
#include "GameManager/Singleton.h"

// define direction
#define D_RIGHT 1
#define D_LEFT -1

// define property character
#define NUM_HERO_TYPE	3
#define NUM_ENERMY_TYPE 3
#define NUM_STATE		6

// define combo key
#define UP		0x01
#define RIGHT	0x02
#define DOWN	0x04
#define LEFT	0x08
#define LSHIFT	0x10

class Constants : public CSingleton<Constants>
{
public:
	Constants::Constants()
	{
		KEY[KEY_MOVE_RIGHT] = RIGHT;
		KEY[KEY_MOVE_LEFT] = LEFT;
		KEY[KEY_MOVE_BACKWORD] = DOWN;
		KEY[KEY_MOVE_FORWORD] = UP;
		KEY[KEY_LSHIFT] = LSHIFT;
	}
	// key combo
	int		KEY[256];
	// hero
	const int		hHEAL[3] = { 100, 150, 200 };
	const int		hWALK_SPEED = 160;
	const int		hRUN_SPEED = 240;
	const int		hDISTANCE_ATTACK[3] = { 425, 300, 175 };
	const int		hDAME[3] = { 30, 40, 50 };
	const float		hTIME_ULTIMATE = 10;
	const float		hTIME_DELAY_ATTACK = 1.0f;
	const float		hTIME_TIRED = 5.0f;
	const float		hTIME_DELAY_TIRED = 2.0f;
	const int		hRADIUS_ULTIMATE = 150;
	const Vector2	hSIZE = { 160, 160 };
	// enermy
	GLint			NUM_ENERMY = 10;
	const GLint		eDAME = 10;
	const GLfloat	eTIME_DELAY_ATTACK = 0.4f;
	const GLint		eSPEED[3] = { 120, 110, 100 };
	const GLint		eHEAL[2] = { 100, 300 };
	const GLint		eDISTANCE_CHASE[2] = { 600, 800 };
	const GLint		eDISTANCE_ATTACK[2] = { 100, 125 };
	const Vector2	eSIZE[2] = { { 141, 96 }, { 211, 144 } };
	// map
	const float		mSCALE = 20.0f;
	const float		mMAP_WIDTH = 3584;
	const float		mMAP_HEIGHT = 3328;
	// arrow
	const int		aSPEED[2] = { 1000, 750 };
	// GS play
	const float		pTIME_RANDOM_ENERMY = 10.0f;
};
