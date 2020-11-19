#pragma once
#include "gamestatebase.h"
#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite3D.h"
#include "Text.h"
#include "GameButton.h"
#include "Player.h"

class Player;
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
	void UpdateSprite2D();
	void UpdateAnim(float deltaTime);
	void UpdateText();

	Vector2 ConvertPosEnermy(Vector2 pos, Vector2 margin);
	Vector2 ConvertPosHero(Vector2 pos, Vector2 margin);
	void RandomEnermy(float deltaTime);
	void EnermyWin();
	void MoveEnermy(Vector2 dVector);
	Vector2 MoveMap(float d);
	void MoveCharacter(float d);
	void InitSprite2Ds();
	void InitGameButtons();
	void InitSpriteAnim();
	void InitText();

	std::shared_ptr<Enermy> FindNearestEnermy();

private:
	bool m_keyPressed, m_isPause, m_isEnd, m_isUltimate;
	int m_keyType;
	float m_time;
	Vector2 m_mapSize, m_miniMapSize;

	std::shared_ptr<Constants> constants;
	std::shared_ptr<Sprite2D> m_grayBG, m_stunHero, m_miniHero;
	std::shared_ptr<Text> m_scoreText;
	std::vector<std::shared_ptr<Text>> m_listText;
	std::vector<std::shared_ptr<Enermy>> m_listEnermy;
	std::vector<std::shared_ptr<GameButton>> m_listButton;
	std::vector<std::shared_ptr<Sprite2D>> m_listSprite2D, m_listMiniEnermy;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listSpriteAnim, m_listHealthBarAnim;
};

