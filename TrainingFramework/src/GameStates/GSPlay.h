#pragma once
#include "gamestatebase.h"

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;
class SpriteAnimation;
class Enermy;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);

	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void HandleMouseMoveEvents(int x, int y);
	void Update(float deltaTime);
	void Draw();

	void MoveEffect(std::shared_ptr<SpriteAnimation> effect, int distance);
	void MoveEnermy(int distance);
	void MoveMap(int distance);
	void MoveCharacter(int distance);
	void SetNewPostionForBullet();

	static void FindNearestEnermy();

private:
	int keyType = 0, m_walk_speed = 160, m_run_speed = 240;
	int miniW, miniH;
	const int MAP_WIDTH = 3813;
	const int MAP_HEIGHT = 3733;
	const int UP = 1;
	const int RIGHT = 2;
	const int DOWN = 4;
	const int LEFT = 8;
	const int LSHIFT = 16;
	const int NUM_OF_ENERMY = 3;
	bool isUsingUlti = false;
	std::shared_ptr<Sprite2D> m_miniBackground, m_BackGround, m_targetScope, m_radiusSkill, miniHero;
	std::shared_ptr<Sprite2D> miniEnermy[3], m_status[6];
	std::shared_ptr<SpriteAnimation> m_killCD[2], m_healthBar[4];
	std::shared_ptr<Text> m_score;
	std::shared_ptr<GameButton> button_play, button_pause, button_ulti;
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite2D;
};

