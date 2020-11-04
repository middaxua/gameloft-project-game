#pragma once
#include "gamestatebase.h"
#include "GameButton.h"

class SpriteAnimation;
class Sprite2D;

class GSCredit :
	public GameStateBase
{
public:
	GSCredit();
	~GSCredit();

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);
	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

private:
	std::shared_ptr<Text>  m_Text_gameName;
	std::list<std::shared_ptr<GameButton>> m_listButton;
	std::shared_ptr<Sprite2D> m_BackGround;
};

