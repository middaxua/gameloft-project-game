#pragma once
#include "gamestatebase.h"

class SpriteAnimation;
class Sprite2D;

class GSIntro :
	public GameStateBase
{
public:
	GSIntro();
	~GSIntro();

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

private:
	std::shared_ptr<SpriteAnimation> m_loading;
	std::shared_ptr<Sprite2D> m_logo;
	float m_time;
};

