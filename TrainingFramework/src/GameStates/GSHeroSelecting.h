#pragma once
#include "gamestatebase.h"
#include <vector>

class SpriteAnimation;
class GameButton;
class Sprite2D;
class Text;

class GSHeroSelecting :
	public GameStateBase
{
public:
	GSHeroSelecting();
	~GSHeroSelecting();

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
	std::vector<std::shared_ptr<SpriteAnimation>> m_SpriteAnimation[3];
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::shared_ptr<Sprite2D> m_BackGround1, m_BackGround2;
	std::shared_ptr<Text> m_Text_gameName;
	bool m_isPause;
};

