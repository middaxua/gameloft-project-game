#include "Player.h"

extern bool isEfMusicOn;
extern bool isTextFloatOn;
extern int idHero;
extern int screenWidth;
extern int screenHeight;
extern int dieSound;
extern int hurtSound;
extern int score;

Player::Player() {
	constants = Constants::GetInstance();
	m_isTired = false;
	m_speed = 0;
	m_flash = 0;
	m_direction = D_RIGHT;
	m_healMax = m_heal = constants->hHEAL[idHero];
	m_dame = constants->hDAME[idHero];
	m_distanceAttack = constants->hDISTANCE_ATTACK[idHero];
	m_timeDelayAttack = constants->hTIME_DELAY_ATTACK;
	m_timeDelayUltimate = constants->hTIME_ULTIMATE;
	m_timeTired = constants->hTIME_TIRED;
	m_timeDelayTired = constants->hTIME_DELAY_TIRED;
	m_radiusUltimate = constants->hRADIUS_ULTIMATE;

	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	// hero
	std::string hStates[6] = { "idle", "walk", "run", "attack", "die", "hurt" };
	std::string hNames[3] = { "archer_", "icewizard_", "knight_" };
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	for (int i = 0; i < 6; ++i) {
		std::string fileName = hNames[idHero] + hStates[i];
		auto texture = ResourceManagers::GetInstance()->GetTexture(fileName);
		std::shared_ptr<SpriteAnimation> heroState = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
		heroState->SetSize(constants->hSIZE);
		m_listHeroState.push_back(heroState);
	}
	m_listHeroState[0]->Set2DPosition(screenWidth / 2, screenHeight / 2 - 60);
	m_activeState = m_listHeroState[0];
	// weapon
	if (idHero != 2)
	{
		std::string equipNames[2] = { "arrow", "thunder_ball" };
		Vector2 size[2] = { {45, 10}, {50, 50} };
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		auto texture = ResourceManagers::GetInstance()->GetTexture(equipNames[idHero]);
		m_weapon = std::make_shared<Sprite2D>(model, shader, texture);
		m_weapon->SetSize(size[idHero]);
		m_weapon->SetEnable(false);
	}
	// health up anim
	auto texture = ResourceManagers::GetInstance()->GetTexture("health_up");
	m_healthUp = std::make_shared<SpriteAnimation>(model, shader, texture, 13, 1.0f / 13);
	m_healthUp->Set2DPosition(Get2DPosition());
	m_healthUp->SetSize(104, 160);
	m_healthUp->SetEnable(false);
	// dame up anim
	texture = ResourceManagers::GetInstance()->GetTexture("dame_up");
	m_dameUp = std::make_shared<SpriteAnimation>(model, shader, texture, 5, 0.2f);
	m_dameUp->Set2DPosition(Get2DPosition());
	m_dameUp->SetSize(120, 120);
	m_dameUp->SetEnable(false);

	if (isTextFloatOn)
	{
		// text dame up
		shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		auto text = std::make_shared< Text>(shader, font, "+30 dmg", TEXT_COLOR::ORANGE, 0.75);
		text->SetEnable(false);
		m_listText.push_back(text);
		// text health up
		text = std::make_shared< Text>(shader, font, "+10 hp", TEXT_COLOR::GREEN, 0.75);
		text->SetEnable(false);
		m_listText.push_back(text);
	}
}

void Player::Update(float deltaTime)
{
	if (IsDie())
	{
		if (!m_activeState->IsLastFrame())
			m_activeState->Update(deltaTime);
	}
	else
	{
		// flash if be hurt
		m_flash = IsHurt() ? (m_flash + 1) % 4 : 0;
		// if attack or hurt anim done, then idle
		if (IsAttack() || IsHurt())
		{
			if (m_activeState->IsLastFrame())
				Idle();
		}
		// update weapon
		UpdateWeapon(deltaTime);
		// update time tired
		UpdateTimeTired(deltaTime);
		// update animation
		UpdateAnim(deltaTime);
		// update kill time cooldown
		m_timeDelayAttack = max(0, m_timeDelayAttack - deltaTime);
		m_timeDelayUltimate = max(0, m_timeDelayUltimate - deltaTime);
		// update
		m_activeState->Update(deltaTime);
	}
}

void Player::UpdateAnim(float deltaTime)
{
	// update anim health up
	if (m_healthUp->IsEnable())
	{
		if (m_healthUp->IsLastFrame())
		{
			m_healthUp->SetEnable(false);
			m_healthUp->ResetCurrentTime();
			m_healthUp->SetCurrentFrame(0);
			if (isTextFloatOn)
				m_listText[1]->SetEnable(false);
		}
		else
			m_healthUp->Update(deltaTime);
	}
	// update anim dame up
	if (m_dameUp->IsEnable())
	{
		if (m_dameUp->IsLastFrame())
		{
			m_dameUp->SetEnable(false);
			m_dameUp->ResetCurrentTime();
			m_dameUp->SetCurrentFrame(0);
			if (isTextFloatOn)
				m_listText[0]->SetEnable(false);
		}
		else
			m_dameUp->Update(deltaTime);
	}
}

void Player::UpdateTimeTired(float deltaTime)
{
	// if run, will tired
	if (IsRun())
	{
		m_timeDelayTired = constants->hTIME_DELAY_TIRED;
		m_timeTired = max(0, m_timeTired - deltaTime);
		if (m_timeTired == 0)
			m_isTired = true;
	}
	else
	{
		m_timeDelayTired = max(0, m_timeDelayTired - deltaTime);
		if (m_timeDelayTired == 0)
		{
			m_timeTired = min(5, m_timeTired + deltaTime);
			if (m_timeTired == 5)
				m_isTired = false;
		}
	}
}

void Player::UpdateWeapon(float deltaTime)
{
	if (idHero != 2)
	{
		// if hero has weapon, update weapon
		if (m_dNeedMoved > 0)
		{
			float d = constants->aSPEED[idHero] * deltaTime;
			Vector2 t = { d * cosf(m_atan), -d * sinf(m_atan) };
			m_dNeedMoved -= t.Length();
			m_weapon->Set2DPosition(m_weapon->Get2DPosition() + t);
		}
		else
		{
			m_weapon->SetEnable(false);
			m_dNeedMoved = 0;
		}
	}
}

void Player::Draw()
{
	if (!IsDie() || !m_activeState->IsLastFrame())
	{
		// draw flash when be hurt
		if (m_flash == 0)
			m_activeState->Draw();
		// draw weapon
		if (idHero != 2)
		{
			if (m_weapon->IsEnable())
				m_weapon->Draw();
		}
		// draw anim
		if (m_healthUp->IsEnable())
			m_healthUp->Draw();
		if (m_dameUp->IsEnable())
			m_dameUp->Draw();
		// draw text
		if (isTextFloatOn)
		{
			for (auto t : m_listText)
			{
				if (t->IsEnable())
					t->Draw();
			}
		}
	}
}

int Player::Attack(std::shared_ptr<Enermy> enermy)
{
	int score = 0;
	m_timeDelayAttack = constants->hTIME_DELAY_ATTACK;
	if (enermy)
	{
		score = enermy->Hurt(m_dame);
		Vector2 dVector = enermy->Get2DPosition() - Get2DPosition();
		int direction = dVector.x > 0 ? D_RIGHT : D_LEFT;
		SetDirection(direction);
		// rotation arrow
		if (idHero != 2)
		{
			m_atan = CalculateAngle(enermy);
			m_weapon->Set2DPosition(Get2DPosition());
			m_weapon->SetRotationZ(m_atan);
			m_weapon->SetEnable(true);
			m_dNeedMoved = dVector.Length();
		}
	}
	ChangeState(HeroStates::hSTATE_Attack);
	return score;
}

void Player::Run()
{
	m_speed = constants->hRUN_SPEED;
	ChangeState(HeroStates::hSTATE_Run);
}

void Player::Walk()
{
	m_speed = constants->hWALK_SPEED;
	ChangeState(HeroStates::hSTATE_Walk);
}

void Player::Idle()
{
	ChangeState(HeroStates::hSTATE_Idle);
}

void Player::Die()
{
	ChangeState(HeroStates::hSTATE_Die);
}

void Player::Hurt(int dame)
{
	m_heal = max(0, m_heal - dame);
	if (m_heal > 0)
	{
		if (isEfMusicOn)
			hurtSound = ResourceManagers::GetInstance()->PlaySound("hurt");
		ChangeState(HeroStates::hSTATE_Hurt);
	}
	else
	{
		if (isEfMusicOn)
			dieSound = ResourceManagers::GetInstance()->PlaySound("die");
		Die();
	}
}

int Player::Ultimate(std::vector<std::shared_ptr<Enermy>> listEnermy, Vector2 ultiPos)
{
	int score = 0;
	m_timeDelayUltimate = constants->hTIME_ULTIMATE;
	Vector2 dVector = (ultiPos - Get2DPosition());
	int direction = dVector.x > 0 ? D_RIGHT : D_LEFT;
	SetDirection(direction);
	ChangeState(HeroStates::hSTATE_Attack);
	for (auto enermy : listEnermy)
	{
		if ((enermy->Get2DPosition() - ultiPos).Length() < m_radiusUltimate)
		{
			score += enermy->Hurt(m_dame);
		}
	}
	return score;
}

void Player::ChangeState(HeroStates hs)
{
	if (m_activeState != m_listHeroState[int(hs)])
	{
		Vector2 oldPos = m_activeState->Get2DPosition();
		m_activeState = m_listHeroState[int(hs)];
		m_activeState->Set2DPosition(oldPos);
		m_activeState->SetCurrentFrame(0);
		m_activeState->ResetCurrentTime();
	}
	m_activeState->SetScaleX(m_direction);
}

int Player::GetDame()
{
	return m_dame;
}

int Player::GetHeal()
{
	return m_heal;
}

int Player::GetHealMax()
{
	return m_healMax;
}

int Player::GetDistanceAttack()
{
	return m_distanceAttack;
}

float Player::GetTimeDelayAttack()
{
	return m_timeDelayAttack;
}

float Player::GetTimeDelayUltimate()
{
	return m_timeDelayUltimate;
}

float Player::GetTimeTired()
{
	return m_timeTired;
}

float Player::CalculateAngle(std::shared_ptr<Enermy> enermy)
{
	Vector2 dVector = enermy->Get2DPosition() - Get2DPosition();
	return atan2(-dVector.y, dVector.x);
}

int Player::GetSpeed()
{
	return m_speed;
}

Vector2 Player::Get2DPosition()
{
	return m_activeState->Get2DPosition();
}

void Player::SetDame(int dame)
{
	m_dame = dame;
}

void Player::SetHeal(int heal)
{
	m_heal = heal;
}

void Player::SetHealMax(int healMax)
{
	m_healMax = healMax;
}

bool Player::IsMove()
{
	return m_activeState == m_listHeroState[1]
		|| m_activeState == m_listHeroState[2];
}

bool Player::IsRun()
{
	return m_activeState == m_listHeroState[2];
}

bool Player::IsDie()
{
	return m_activeState == m_listHeroState[4];
}

bool Player::IsHurt()
{
	return m_activeState == m_listHeroState[5];
}

bool Player::IsAttack()
{
	return m_activeState == m_listHeroState[3];
}

bool Player::IsTired()
{
	return m_isTired;
}

bool Player::NeedTimeTired()
{
	return m_timeTired == 0;
}

bool Player::HasUltimate()
{
	return m_timeDelayUltimate == 0;
}

bool Player::IsLastFrame()
{
	return m_activeState->IsLastFrame();
}

bool Player::Attackable()
{
	return m_timeDelayAttack == 0;
}

void Player::SetDirection(int direction)
{
	m_direction = direction;
}

void Player::DameUp()
{
	int dDame = 0.2f * constants->hDAME[idHero] + 0.05f * score;
	m_dame += dDame;

	m_dameUp->SetEnable(true);
	if (isTextFloatOn)
	{
		m_listText[0]->SetEnable(true);
		m_listText[0]->Set2DPosition(Get2DPosition() - Vector2(100, 50));
		m_listText[0]->setText("+" + std::to_string(dDame) + " dmg");
	}
}

void Player::HealthUp()
{
	// inc health max
	int dHeal = 0.1f * score;
	m_healMax += dHeal;
	// health up
	int healUp = m_heal + dHeal + 25;
	m_heal = min(healUp, m_healMax);
	if (isTextFloatOn)
	{
		m_healthUp->SetEnable(true);
		m_listText[1]->SetEnable(true);
		m_listText[1]->Set2DPosition(Get2DPosition() - Vector2(100, 50));
		m_listText[1]->setText("+" + std::to_string(dHeal + 25) + " hp");
	}
}
