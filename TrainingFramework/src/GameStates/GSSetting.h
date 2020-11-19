#pragma once
#include "gamestatebase.h"
#include "GameButton.h"
#include "Sprite2D.h"
#include "GameHelper/Constants.h"

class GameButton;
class Sprite2D;

class GSSetting : public GameStateBase
{
public:
	GSSetting();
	~GSSetting();

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
	float v;
	std::shared_ptr<Constants> constants;
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite2D;
	std::vector<std::shared_ptr<GameButton>> m_listButton;
};