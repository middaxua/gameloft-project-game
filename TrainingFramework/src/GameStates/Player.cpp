#include "Player.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "Enermy.h"

extern int idHero;
extern bool isMusicOn, isPause, isEnd;
extern int screenWidth, screenHeight;
extern int NUM_HEROES, NUM_STATES;
extern std::vector<std::shared_ptr<Enermy>> m_listEnermy;
Vector2 posAttackEffect[3] = { {17,10},{0,0},{50,0} };

Player::Player() {
	m_timeAttack = m_timeHurt = -1;
	m_timeTired = 5;
	m_timeDie = 1;
	m_timeUltimate = 10;
	m_timeDisplayEffectUltimate = -1;
	m_heal = 100;
	m_isDie = m_isAttacking = m_isTired = false;

	constants = Constants::GetInstance();
	m_listState[0].clear();
	m_listState[1].clear();
	// init hero
	std::string listHeroStates[7] = { "0", "1", "2", "3", "4", "5", "7" };
	std::string listHeroNames[3] = { "archer_", "icewizard_", "knight_" };
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	// right
	for (int j = 0; j < NUM_STATES; ++j) {
		std::string fileName = listHeroNames[idHero] + listHeroStates[j] + "_0";
		auto texture = ResourceManagers::GetInstance()->GetTexture(fileName);
		std::shared_ptr<SpriteAnimation> heroState = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
		heroState->Set2DPosition(240, 300);
		heroState->SetSize(128, 128);
		m_listState[0].push_back(heroState);
	}
	// left
	for (int j = 0; j < NUM_STATES; ++j) {
		std::string fileName = listHeroNames[idHero] + listHeroStates[j] + "_1";
		auto texture = ResourceManagers::GetInstance()->GetTexture(fileName);
		std::shared_ptr<SpriteAnimation> heroState = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
		heroState->Set2DPosition(240, 300);
		heroState->SetSize(128, 128);
		m_listState[1].push_back(heroState);
	}
	m_pActiveState = m_listState[0][0];

	// animation explosion attack
	int numberFrame[3] = { 25,36,5 };
	int size[3][2] = { {96, 96}, {128, 128}, {77, 77} };
	for (int i = 0; i < 3; ++i) {
		auto texture = ResourceManagers::GetInstance()->GetTexture("effect_attack_" + std::to_string(i));
		m_pExplosion[i] = std::make_shared<SpriteAnimation>(model, shader, texture, numberFrame[i], 1.0 / numberFrame[i]);
		m_pExplosion[i]->SetSize(size[i][0], size[i][1]);
	}

	// effect ultimate
	auto texture = ResourceManagers::GetInstance()->GetTexture("effect_ulti_" + std::to_string(idHero));
	m_effectUltimate = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 1.0 / 4);
	m_effectUltimate->SetSize(300, 300);

	// arrow for archer (id = 0)
	if (idHero == 0)
	{
		shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		for (int i = 0; i < 4; ++i)
		{
			texture = ResourceManagers::GetInstance()->GetTexture("arrow_" + std::to_string(i));
			m_arrow[i] = std::make_shared<Sprite2D>(model, shader, texture);
			if (i % 2 == 0)
				m_arrow[i]->SetSize(45, 10);
			else
				m_arrow[i]->SetSize(10, 45);
		}
	}
}

Player::~Player()
{
}

void Player::Update(float deltaTime)
{
	m_timeUltimate = max(0, m_timeUltimate - deltaTime);
	if (m_timeDisplayEffectUltimate > 0)
	{
		m_effectUltimate->Update(deltaTime);
		m_timeDisplayEffectUltimate -= deltaTime;
	}
	else
	{
		m_timeDisplayEffectUltimate = -1;
	}
	if (m_isAttacking)
	{
		if (m_timeAttack < 0) {
			m_isAttacking = false;
			m_timeAttack = -1;
			m_pActiveState = m_listState[m_direction][0];
		}
		else
		{
			m_pExplosion[idHero]->Update(deltaTime);
			if (idHero == 0)
				m_arrow[m_arrowId]->Set2DPosition(m_arrow[m_arrowId]->Get2DPosition() + distanceHeroEnermy * deltaTime);
			m_timeAttack -= deltaTime;
		}
	}
	if (m_timeHurt < 0)
	{
		if (m_timeHurt != -1)
		{
			m_timeHurt = -1;
			m_pActiveState = m_listState[m_direction][0];
		}
	}
	else
	{
		m_timeHurt -= deltaTime;
	}
	if (m_isDie)
	{
		if (m_timeDie >= 0) {
			m_timeDie -= deltaTime;
			m_pActiveState->Update(deltaTime);
		}
		else
		{
			m_timeDie = -1;
			isPause = isEnd = true;
			if (isMusicOn)
			{
				ResourceManagers::GetInstance()->PauseSound("bgsound_play");
				ResourceManagers::GetInstance()->PlaySound("endgame");
			}
		}
	}
	else m_pActiveState->Update(deltaTime);
}

void Player::Draw()
{
	if (m_timeDie > 0)
	{
		m_pActiveState->Draw();
		if (m_isAttacking)
		{
			m_pExplosion[idHero]->Draw();
			if (idHero == 0)
				m_arrow[m_arrowId]->Draw();
		}
	}
}

void Player::Attack(int id)
{
	if (!m_isAttacking) {
		if (isMusicOn)
		ResourceManagers::GetInstance()->PlaySound("attack");
		if (id > -1) {
			if (idHero == 0)
			{
				// caculate tan enermy and hero
				Vector2 heroPos = m_pActiveState->Get2DPosition();
				Vector2 enermyPos = m_listEnermy[id]->CurrentState()->Get2DPosition();
				float tan = 999999;
				if (enermyPos.x - heroPos.x != 0)
					tan = (enermyPos.y - heroPos.y) / (enermyPos.x - heroPos.x);
				distanceHeroEnermy = enermyPos - heroPos;
				m_direction = (distanceHeroEnermy.x > 0) ? 0 : 1;
				if (tan > 1)
					m_arrowId = (distanceHeroEnermy.x > 0) ? 1 : 3;
				else if (tan > 0)
					m_arrowId = (distanceHeroEnermy.x > 0) ? 0 : 2;
				else if (tan < -1)
					m_arrowId = (distanceHeroEnermy.x > 0) ? 3 : 1;
				else
					m_arrowId = (distanceHeroEnermy.x > 0) ? 0 : 2;
				m_arrow[m_arrowId]->Set2DPosition(heroPos);
			}
			// gay sat thuong len enermy
			m_listEnermy[id]->Hurt(m_dame[idHero]);
		}
		m_isAttacking = true;
		m_timeAttack = 1;
		m_pActiveState = m_listState[m_direction][3];
		m_pActiveState->SetCurrentFrame(0);
		m_pExplosion[idHero]->Set2DPosition(m_pActiveState->Get2DPosition() + posAttackEffect[idHero]);
	}
}

void Player::Run()
{
	ChangeState(HeroStates::STATE_Run);
}

void Player::Walk()
{
	ChangeState(HeroStates::STATE_Walk);
}

void Player::Idle()
{
	ChangeState(HeroStates::STATE_Idle);
}

void Player::Die()
{
	if (isMusicOn)
		ResourceManagers::GetInstance()->PlaySound("die");
	m_isDie = true;
	ChangeState(HeroStates::STATE_Die);
}

void Player::Hurt(int dame)
{
	m_heal = max(0, m_heal - dame);
	if (m_heal > 0)
	{
		if (isMusicOn)
			ResourceManagers::GetInstance()->PlaySound("hurt");
		ChangeState(HeroStates::STATE_Hurt);
	}
	else
	{
		Die();
		for (auto enermy : m_listEnermy)
		{
			if (!enermy->m_isDie)
			{
				enermy->Win();
			}
		}
	}
}

void Player::Ultimate(Vector2 centerPos)
{
	if (isMusicOn)
		ResourceManagers::GetInstance()->PlaySound("ultimate");
	ResetTimeUltimate(constants->TIME_ULTIMATE);
	m_effectUltimate->SetCurrentFrame(0);
	m_effectUltimate->Set2DPosition(centerPos);
	m_timeDisplayEffectUltimate = 1;
	for (auto enermy : m_listEnermy)
	{
		Vector2 enermyPos = enermy->Get2DPosition();
		if (!enermy->m_isDie)
		{
			int distance = (enermyPos - centerPos).Length();
			if (distance < constants->RADIUS_ULTIMATE)
			{
				enermy->Hurt(m_dame[idHero]);
			}
		}
	}
}

void Player::ChangeState(HeroStates hs)
{
	m_pActiveState = m_listState[m_direction][int(hs)];
}

void Player::ResetTimeUltimate(float time)
{
	m_timeUltimate = time;
}

Vector2 Player::Get2DPosition()
{
	return m_pActiveState->Get2DPosition();
}

bool Player::IsMove()
{
	return m_pActiveState == m_listState[m_direction][int(HeroStates::STATE_Walk)]
		|| m_pActiveState == m_listState[m_direction][int(HeroStates::STATE_Run)];
}
