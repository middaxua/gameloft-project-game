#pragma once
#include "GameManager/Singleton.h"
#include "GameConfig.h"
#include "GameHelper/Constants.h"
#include "ResourceManagers.h"
#include <vector>

class Sprite2D;
class SpriteAnimation;

enum HeroStates {
	STATE_Idle = 0,
	STATE_Walk,
	STATE_Run,
	STATE_Attack,
	STATE_Win,
	STATE_Die,
	STATE_Hurt
};

class Player : public CSingleton<Player>
{

public:
	Player();
	~Player();

public:
	inline std::shared_ptr<SpriteAnimation>	CurrentState()const
	{
		return m_pActiveState;
	}

	void Update(float deltaTime);
	void Draw();

	void Attack(int id);
	void Run();
	void Walk();
	void Idle();
	void Die();
	void Hurt(int dame);
	void Ultimate(Vector2 centerPos);
	void ChangeState(HeroStates hs);
	void ResetTimeUltimate(float time);
	Vector2 Get2DPosition();
	
	bool IsMove();

	GLint m_distanceAttack[3] = { 300, 225, 150 };
	GLint m_dame[3] = { 30, 40, 50 };
	GLfloat m_timeTired, m_timeUltimate, m_timeDisplayEffectUltimate;
	bool m_isDie, m_isAttacking, m_isTired;
	std::shared_ptr<SpriteAnimation> m_effectUltimate;
	GLint m_heal, m_direction = 0;
	GLfloat m_timeAttack, m_timeDie, m_timeHurt;

private:
	int m_arrowId = 0;
	Vector2 distanceHeroEnermy;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listState[2];
	std::shared_ptr<Sprite2D> m_arrow[4];
	std::shared_ptr<Sprite2D> m_skill;
	std::shared_ptr<SpriteAnimation> m_pActiveState, m_pExplosion[3];
	std::shared_ptr<Constants> constants;
};