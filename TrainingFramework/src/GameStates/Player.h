#pragma once
#include "GameConfig.h"
#include "ResourceManagers.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "Enermy.h"
#include <vector>

class Enermy;
class Sprite2D;
class SpriteAnimation;
class Constants;

enum HeroStates {
	hSTATE_Idle = 0,
	hSTATE_Walk,
	hSTATE_Run,
	hSTATE_Attack,
	hSTATE_Die,
	hSTATE_Hurt
};

class Player
{

public:
	Player();
	~Player();

public:
	void Init();
	void Update(float deltaTime);
	void UpdateAnim(float deltaTime);
	void UpdateTimeTired(float deltaTime);
	void UpdateWeapon(float deltaTime);
	void Draw();

	int Attack(std::shared_ptr<Enermy> enermy);
	void Run();
	void Walk();
	void Idle();
	void Die();
	void Hurt(int dame);
	int Ultimate(std::vector<std::shared_ptr<Enermy>> listEnermy, Vector2 ultiPos);
	void ChangeState(HeroStates hs);
	
	bool	IsMove();
	bool	IsRun();
	bool	IsDie();
	bool	IsHurt();
	bool	IsAttack();
	bool	IsTired();
	bool	NeedTimeTired();
	bool	HasUltimate();
	bool	IsLastFrame();
	bool	Attackable();
	int		GetSpeed();
	int		GetDame();
	int		GetHeal();
	int		GetHealMax();
	int		GetDistanceAttack();
	float	GetTimeDelayAttack();
	float	GetTimeDelayUltimate();
	float	GetTimeTired();
	float	CalculateAngle(std::shared_ptr<Enermy> enermy);
	Vector2 Get2DPosition();

	void	SetDame(int dame);
	void	SetHeal(int heal);
	void	SetHealMax(int healMax);
	void	SetDirection(int direction);
	void	DameUp();
	void	HealthUp();

private:
	bool m_isTired;
	GLint m_healMax;
	GLint m_heal, m_speed, m_dame, m_direction;
	GLint m_distanceAttack, m_radiusUltimate, m_flash;
	GLfloat m_timeDelayAttack, m_timeDelayUltimate;
	GLfloat m_timeDelayTired, m_timeTired, m_atan, m_dNeedMoved;
	std::shared_ptr<Sprite2D> m_weapon;
	std::shared_ptr<Constants> constants;
	std::shared_ptr<SpriteAnimation> m_activeState, m_healthUp, m_dameUp;
	std::vector<std::shared_ptr<Text>> m_listText;
	std::vector<std::shared_ptr<SpriteAnimation>> m_listHeroState;
};
