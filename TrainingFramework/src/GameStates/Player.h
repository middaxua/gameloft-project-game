#pragma once
#include "GameManager/Singleton.h"
#include "GameConfig.h"
#include "gamestatebase.h"
#include <vector>

class Sprite2D;
class SpriteAnimation;

class Player : public GameStateBase, public CSingleton<Player>
{

public:
	Player();
	~Player();

public:
	inline std::shared_ptr<SpriteAnimation>	CurrentState()const
	{
		return m_pActiveState;
	}

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

	void Attack(int id);
	void Run();
	void Walk();
	void Idle();
	void Die();
	void Hurt(int dame);
	void Ultimate(float x, float y);
	
	bool IsMove();
	//void CheckCollision();

	GLint m_distanceAttack[3] = { 300, 225, 150 };
	GLint m_dame[3] = { 30, 40, 50 };
	GLfloat m_timeTired, m_timeUltimate, m_timeDisplayEffectUltimate;
	bool m_isDie, m_isAttacking, m_isTired;
	std::shared_ptr<SpriteAnimation> m_effectUltimate;
	GLint m_heal, m_direction;
	GLfloat m_timeAttack, m_timeDie, m_timeHurt;

private:
	char **arrayCharacterName, *arrayEffectAttackName;
	int m_arrowId = 0;
	Vector2 distanceHeroEnermy;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listState;
	std::shared_ptr<Sprite2D> m_arrow[4];
	std::shared_ptr<Sprite2D> m_skill;
	std::shared_ptr<SpriteAnimation> m_pActiveState, m_pExplosion[3];
};