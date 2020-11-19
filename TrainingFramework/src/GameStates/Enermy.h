#pragma once
#include "GameConfig.h"
#include "ResourceManagers.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "Player.h"
#include "GameHelper/Constants.h"
#include <vector>
#include <ctime>

class Player;
class Constants;

enum EnermyStates {
	eSTATE_Idle = 0,
	eSTATE_Walk,
	eSTATE_Attack,
	eSTATE_Win,
	eSTATE_Die,
	eSTATE_Hurt
};

class Enermy
{

public:
	Enermy();
	~Enermy();

public:
	void Init();
	void Update(std::shared_ptr<Player> hero, float deltaTime);
	void Draw();

	void Idle();
	void Walk();
	void Attack(std::shared_ptr<Player> hero, int direction);
	void Win();
	void Die();
	int Hurt(int dame);
	void ChangeState(EnermyStates es);
	void Reset(int score);

	bool IsDie();
	bool IsAttack();
	bool IsWin();
	bool IsHurt();
	bool IsBoss();
	int GetHeal();
	int GetHealMax();
	std::string GetStringHeal();
	bool Chasable(int d);
	bool Attackable(int d, float deltaTime);
	void ChaseHero(Vector2 dVector, float d);

	void SetDirection(int direction);
	void Set2DPosition(Vector2 pos);

	Vector2 Get2DPosition();

private:
	bool m_isBoss;
	GLint m_healMax;
	std::string m_strHeal;
	GLint m_heal, m_speed, m_dame, m_direction, m_flash;
	GLint m_distanceChase, m_distanceAttack;
	GLfloat m_timeDelayAttack;
	std::shared_ptr<Constants> constants;
	std::shared_ptr<SpriteAnimation> m_activeState, m_explosion;
	std::vector<std::shared_ptr<Text>> m_listText;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listEnermyState;
};