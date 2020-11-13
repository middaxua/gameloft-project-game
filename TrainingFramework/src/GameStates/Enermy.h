#pragma once
#include "GameConfig.h"
#include "ResourceManagers.h"
#include <vector>

class Sprite2D;
class SpriteAnimation;

class Enermy
{

public:
	Enermy(int id);
	~Enermy();

public:
	inline std::shared_ptr<SpriteAnimation>	CurrentState()const
	{
		return m_pActiveState;
	}

	void Update(float deltaTime);
	void Draw();

	void Attack();
	void Walk();
	void Idle();
	void Die();
	void Win();
	void Hurt(int dame);

	void ChaseHero(Vector2 distanceVector, float d);
	void Move(Vector2 distanceVector, float d);
	void PrepareAttack(Vector2 distanceVector, float deltaTime);
	Vector2 Get2DPosition();

	bool m_isDie, m_isAttacking, isBoss;
	GLint m_heal, m_direction, m_distanceChase, m_distanceAttack, m_speed = 60, m_dame = 10;
private:
	char **arrayCharacterName;
	GLint flash = 0;
	GLfloat m_timeAttack, m_timeDie, m_timeHurt, m_countToAttack = 0.5;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listState;
	std::vector<std::shared_ptr<Sprite2D>> m_listBullet;
	std::shared_ptr<Sprite2D> m_skill;
	std::shared_ptr<SpriteAnimation> m_pActiveState, m_pExplosionDie, m_pEffectHurt[3];
};