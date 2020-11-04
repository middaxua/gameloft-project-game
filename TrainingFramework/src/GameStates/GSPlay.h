#pragma once
#include "gamestatebase.h"

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;
class SpriteAnimation;
class CharacterState;

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
	void Update(float deltaTime);
	void Draw();

	
	void SetNewPostionForBullet();

private:
	const int MAP_WIDTH = 3072;
	const int MAP_HEIGHT = 2048;
	std::shared_ptr<Sprite2D> m_BackGround;
	std::shared_ptr<Text> m_score;
	std::shared_ptr<CharacterState> m_hero;
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite2D;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listSpriteAnimations;
};

