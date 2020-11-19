#include "LoadingHelper.h"

LoadingHelper::LoadingHelper()
{
	rm = ResourceManagers::GetInstance();
	rm->AddModel("Sprite2D");
	rm->AddShader("Animation");
	rm->AddShader("TextShader");
	rm->AddShader("TextureShader");
}

LoadingHelper::~LoadingHelper()
{
}

void LoadingHelper::LoadingIntro()
{
	rm->AddTexture("loading_game");
	rm->AddTexture("logo");
	// bg game play
	rm->AddTexture("bg_game");
}

void LoadingHelper::LoadingMenu()
{
	rm->AddTexture("bg_paralel_menu");
	rm->AddTexture("button_play_2");
	rm->AddTexture("button_music");
	rm->AddTexture("button_mute");
	rm->AddTexture("button_credit");
	rm->AddTexture("button_quit_2");
	rm->AddTexture("title_game");
}

void LoadingHelper::LoadingCredit()
{

}

void LoadingHelper::LoadingHeroSelecting(int numHeroes, int numStates)
{
	// hero
	std::string listHeroStates[7] = { "0", "1", "2", "3", "4", "5", "7" };
	std::string listHeroNames[3] = { "archer_", "icewizard_", "knight_" };
	for (int i = 0; i < numHeroes; ++i) {
		for (int j = 0; j < numStates; ++j) {
			std::string fileName = listHeroNames[i] + listHeroStates[j];
			rm->AddTexture(fileName);
		}
	}
	rm->AddTexture("transparent");
}

void LoadingHelper::LoadingPlay()
{
	rm->AddTexture("target_scope");
	rm->AddTexture("mini_hero");
}

void LoadingHelper::LoadingMusic()
{
	rm->AddSound("intro");
	rm->AddSound("menu");
	rm->AddSound("credit");
	rm->AddSound("select_hero");
	rm->AddSound("bgsound_play");
	rm->AddSound("attack");
	rm->AddSound("die");
	rm->AddSound("hurt");
	rm->AddSound("endgame");
	rm->AddSound("ultimate");
}
