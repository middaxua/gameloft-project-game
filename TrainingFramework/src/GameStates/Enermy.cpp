#include "Enermy.h"

extern int screenWidth;
extern int screenHeight;
extern int idHero;
extern bool isTextFloatOn;
extern std::shared_ptr<Player> m_player;
extern std::shared_ptr<Sprite2D> m_background;

Enermy::Enermy()
{
	constants = Constants::GetInstance();
	m_speed = constants->eSPEED[idHero];
	m_dame = constants->eDAME;
	m_flash = 0;
	m_direction = rand() % 2 ? D_RIGHT : D_LEFT;
	m_isBoss = rand() % 3 == 0;
	m_healMax = m_heal = constants->eHEAL[m_isBoss];
	m_strHeal = "/" + std::to_string(m_healMax);
	m_distanceChase = constants->eDISTANCE_CHASE[m_isBoss];
	m_distanceAttack = constants->eDISTANCE_ATTACK[m_isBoss];
	m_timeDelayAttack = 2.5f;
	
	Init();
}

Enermy::~Enermy() {}

void Enermy::Init()
{
	int id = rand() % 3;
	std::string eStates[6] = { "idle", "walk", "attack", "win", "die", "hurt" };
	std::string eNames[3] = { "minotaur_01_", "minotaur_02_", "minotaur_03_" };
	int eNumFrames[6] = { 12, 18, 12, 18, 15,12 };
	float eTimeFrames[6] = { 1.0f, 1.0f, 0.4f, 1.0f, 1.0f, 1.0f };
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	for (int i = 0; i < 6; ++i) {
		std::string fileName = eNames[id] + eStates[i];
		auto texture = ResourceManagers::GetInstance()->GetTexture(fileName);
		std::shared_ptr<SpriteAnimation> enermyState = std::make_shared<SpriteAnimation>(model, shader, texture, eNumFrames[i], eTimeFrames[i] / eNumFrames[i]);
		enermyState->SetSize(constants->eSIZE[m_isBoss]);
		m_listEnermyState.push_back(enermyState);
	}
	// init first position
	float rx = rand() % (int)(constants->mMAP_WIDTH - screenWidth) - (constants->mMAP_WIDTH - screenWidth) * 0.5;
	float ry = rand() % (int)(constants->mMAP_HEIGHT - screenHeight) - (constants->mMAP_HEIGHT - screenHeight) * 0.5;
	m_listEnermyState[0]->Set2DPosition(Vector2(rx, ry) + m_background->Get2DPosition());
	m_activeState = m_listEnermyState[0];
	// explosion
	auto texture = ResourceManagers::GetInstance()->GetTexture("hurt");
	m_explosion = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.25f);
	m_explosion->SetSize(50, 125);
	
	if (isTextFloatOn)
	{
		// text hurt [0]
		shader = ResourceManagers::GetInstance()->GetShader("TextShader");
		std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
		auto text = std::make_shared< Text>(shader, font, "-30 hp", TEXT_COLOR::BLUE, 0.75);
		text->SetEnable(false);
		m_listText.push_back(text);
	}
}

void Enermy::Update(std::shared_ptr<Player> hero, float deltaTime)
{
	if (IsDie())
	{
		if (!m_activeState->IsLastFrame())
			m_activeState->Update(deltaTime);
	}
	else
	{
		if (!IsWin())
		{
			if (IsAttack() || IsHurt())
			{
				// update explosion and text hurt
				if (IsHurt())
				{
					m_explosion->Update(deltaTime);
					if (isTextFloatOn)
					{
						if (m_activeState->IsLastFrame())
							m_listText[0]->SetEnable(false);
					}
				}
				if (m_activeState->IsLastFrame())
					Idle();
			}
			else
			{
				Vector2 dVector = hero->Get2DPosition() - Get2DPosition();
				int d = dVector.Length();
				if (Attackable(d, deltaTime))
				{
					int direction = dVector.x > 0 ? D_RIGHT : D_LEFT;
					Attack(hero, direction);
				}
				else if (Chasable(d))
				{
					ChaseHero(dVector, deltaTime * m_speed);
				}
				else Idle();
			}
			// flash if enermy is hurt
			m_flash = IsHurt() ? (m_flash + 1) % 2 : 0;
			// update time attack
			m_timeDelayAttack = max(0, m_timeDelayAttack - deltaTime);
		}
		m_activeState->Update(deltaTime);
	}
}

void Enermy::Draw()
{
	if (!IsDie() || !m_activeState->IsLastFrame())
	{
		if (m_flash == 0)
			m_activeState->Draw();
		if (IsHurt())
			m_explosion->Draw();
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

void Enermy::Idle()
{
	ChangeState(EnermyStates::eSTATE_Idle);
}

void Enermy::Walk()
{
	ChangeState(EnermyStates::eSTATE_Walk);
}

void Enermy::Attack(std::shared_ptr<Player> hero, int direction)
{
	hero->Hurt(m_dame);
	ChangeState(EnermyStates::eSTATE_Attack);
	m_timeDelayAttack = constants->eTIME_DELAY_ATTACK;
	SetDirection(direction);
}

void Enermy::Die()
{
	if (isTextFloatOn)
		m_listText[0]->SetEnable(false);
	ChangeState(EnermyStates::eSTATE_Die);
	// hero reward
	switch (rand() % 4)
	{
	case 0:
		m_player->HealthUp();
		break;
	case 1:
		m_player->DameUp();
		break;
	}
}

void Enermy::Win()
{
	ChangeState(EnermyStates::eSTATE_Win);
}

int Enermy::Hurt(int dame)
{
	int score = 0;
	m_heal = max(0, m_heal - dame);
	if (m_heal > 0)
	{
		ChangeState(EnermyStates::eSTATE_Hurt);
		m_explosion->Set2DPosition(Get2DPosition());
		if (isTextFloatOn)
		{
			m_listText[0]->SetEnable(true);
			m_listText[0]->Set2DPosition(Get2DPosition() - Vector2(100, 50));
			m_listText[0]->setText("-" + std::to_string(dame) + " hp");
		}
	}
	else
	{
		Die();
		score = m_isBoss ? 40 : 10;
	}
	return score;
}

void Enermy::ChangeState(EnermyStates es)
{
	if (m_activeState != m_listEnermyState[int(es)])
	{
		Vector2 oldPos = m_activeState->Get2DPosition();
		m_activeState = m_listEnermyState[int(es)];
		m_activeState->Set2DPosition(oldPos);
		m_activeState->SetCurrentFrame(0);
		m_activeState->ResetCurrentTime();
	}
	m_activeState->SetScaleX(m_direction);
}

void Enermy::Reset(int score)
{
	m_speed = min(constants->eSPEED[idHero] + 0.1f * score, constants->hRUN_SPEED);
	m_dame = constants->eDAME + 0.2f * score;
	m_flash = 0;
	m_direction = rand() % 2 ? D_RIGHT : D_LEFT;
	m_isBoss = rand() % 3 == 0;
	m_healMax = m_heal = constants->eHEAL[m_isBoss] + 0.75f * score;
	m_strHeal = "/" + std::to_string(m_healMax);
	m_distanceChase = constants->eDISTANCE_CHASE[m_isBoss] + 0.5f * score;
	m_distanceAttack = constants->eDISTANCE_ATTACK[m_isBoss];
	m_timeDelayAttack = 0.25f;

	m_listEnermyState.clear();
	Init();
}

bool Enermy::IsDie()
{
	return m_activeState == m_listEnermyState[4];
}

bool Enermy::IsAttack()
{
	return m_activeState == m_listEnermyState[2];
}

bool Enermy::IsWin()
{
	return m_activeState == m_listEnermyState[3];
}

bool Enermy::IsHurt()
{
	return m_activeState == m_listEnermyState[5];
}

bool Enermy::IsBoss()
{
	return m_isBoss;
}

int Enermy::GetHeal()
{
	return m_heal;
}

int Enermy::GetHealMax()
{
	return m_healMax;
}

std::string Enermy::GetStringHeal()
{
	return std::to_string(m_heal) + m_strHeal;
}

bool Enermy::Chasable(int d)
{
	return d <= m_distanceChase && d > m_distanceAttack;
}

bool Enermy::Attackable(int d, float deltaTime)
{
	return d <= m_distanceAttack && m_timeDelayAttack == 0;
}

void Enermy::ChaseHero(Vector2 dVector, float d)
{
	Walk();
	Vector2 ePos = m_activeState->Get2DPosition();
	if (abs(dVector.x) > abs(dVector.y))
	{
		if (dVector.x > 0)
			m_activeState->Set2DPosition(ePos.x + d, ePos.y);
		else
			m_activeState->Set2DPosition(ePos.x - d, ePos.y);
	}
	else
	{
		if (dVector.y > 0)
			m_activeState->Set2DPosition(ePos.x, ePos.y + d);
		else
			m_activeState->Set2DPosition(ePos.x, ePos.y - d);
	}
	int direction = dVector.x > 0 ? D_RIGHT : D_LEFT;
	SetDirection(direction);
}

void Enermy::SetDirection(int direction)
{
	m_direction = direction;
}

void Enermy::Set2DPosition(Vector2 pos)
{
	m_activeState->Set2DPosition(pos);
}

Vector2 Enermy::Get2DPosition()
{
	return m_activeState->Get2DPosition();
}
