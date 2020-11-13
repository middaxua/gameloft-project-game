#pragma once
#include "GameManager/Singleton.h"
#include "ResourceManagers.h"

class LoadingHelper: public CSingleton<LoadingHelper>
{
public:
	LoadingHelper();
	~LoadingHelper();

	void LoadingIntro();
	void LoadingMenu();
	void LoadingCredit();
	void LoadingHeroSelecting(int numHeroes, int numStates);
	void LoadingPlay();
	void LoadingMusic();

private:
	std::shared_ptr<ResourceManagers> rm;
};