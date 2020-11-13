#pragma once
#include "GameManager/Singleton.h"

class Constants : public CSingleton<Constants>
{
public:
	Constants();
	~Constants();

public:
	const int DISTANCE_ATTACK[3] = { 300, 225, 150 };
	const int DAME[3] = { 30, 40, 50 };
	const float TIME_ULTIMATE = 10;
	const int RADIUS_ULTIMATE = 150;
};
