#include "Enermy.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "Player.h"

extern int idHero;
extern int score;

Enermy::Enermy(int id) {
	isBoss = false;
	m_timeAttack = m_timeHurt = -1;
	m_timeDie = 1;
	m_heal = 100;
	m_distanceChase = 300; m_distanceAttack = 70;
	m_isDie = false;
	m_isAttacking = false;

	arrayCharacterName = new char*[3];
	arrayCharacterName[0] = new char[15]{ "minotaur_01_0" };
	arrayCharacterName[1] = new char[15]{ "minotaur_02_0" };
	arrayCharacterName[2] = new char[15]{ "minotaur_03_0" };

	std::shared_ptr<SpriteAnimation> c = nullptr;
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	std::shared_ptr<Texture> texture = nullptr;
	std::shared_ptr<SpriteAnimation> tmpCharacter = nullptr;
	int len = strlen(arrayCharacterName[id]);
	int numberFrame[] = { 12, 18, 12,12,18,15,12,12 };
	for (int i = 0; i < 8; ++i) {
		arrayCharacterName[id][len - 1] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(arrayCharacterName[id]);
		tmpCharacter = std::make_shared<SpriteAnimation>(model, shader, texture, numberFrame[i], 1.0 / numberFrame[i]);
		tmpCharacter->SetSize(141, 96);
		m_listState.push_back(tmpCharacter);
	}
	m_pActiveState = m_listState[0];

	// animation explosion die
	texture = ResourceManagers::GetInstance()->GetTexture("explosion_blue");
	m_pExplosionDie = std::make_shared<SpriteAnimation>(model, shader, texture, 9, 1.0 / 9);
	m_pExplosionDie->SetSize(96, 60);

	// animation effect hurt
	char effectHurtName[] = "effect_hurt_0";
	int lenEff = strlen(effectHurtName);
	int numberFrameEffect[3] = { 9,4,16 };
	int size[3][2] = { {96, 71}, {39, 96}, {93, 96} };
	for (int i = 0; i < 3; ++i)
	{
		effectHurtName[lenEff - 1] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(effectHurtName);
		m_pEffectHurt[i] = std::make_shared<SpriteAnimation>(model, shader, texture, numberFrameEffect[i], 1.0 / numberFrameEffect[i]);
		m_pEffectHurt[i]->SetSize(size[i][0], size[i][1]);
	}
}

Enermy::~Enermy() {
	for (int i = 0; i < 3; ++i)
	{
		delete[] arrayCharacterName[i];
	}
	delete[] arrayCharacterName;
}

void Enermy::Update(float deltaTime)
{
	if (Player::GetInstance()->m_isDie)
		m_pActiveState->Update(deltaTime);
	else
	{
		if (m_isAttacking)
		{
			if (m_timeAttack < 0) {
				m_isAttacking = false;
				m_timeAttack = -1;
				m_listState[0]->Set2DPosition(m_pActiveState->Get2DPosition());
				m_pActiveState = m_listState[0];
			}
			else
			{
				m_timeAttack -= deltaTime;
			}
		}
		if (m_timeHurt < 0)
		{
			// Vua het thoi gian bi danh thuong
			if (m_timeHurt != -1) {
				m_timeHurt = -1;
				//m_listState[0]->Set2DPosition(m_pActiveState->Get2DPosition());
				//m_pActiveState = m_listState[0];
				flash = 0;
			}
		}
		else { // Neu bi thuong
			m_pEffectHurt[idHero]->Update(deltaTime);
			m_timeHurt -= deltaTime;
			flash = (flash + 1) % 4;
		}
		if (m_isDie)
		{
			if (m_timeDie >= 0) {
				m_timeDie -= deltaTime;
				m_pActiveState->Update(deltaTime);
				m_pExplosionDie->Update(deltaTime);
			}
			else m_timeDie = -1;
		}
		else {
			Vector2 heroPos = Player::GetInstance()->CurrentState()->Get2DPosition();
			Vector2 enermyPos = m_pActiveState->Get2DPosition();
			Vector2 distanceVector = heroPos - enermyPos;
			// chase hero
			ChaseHero(distanceVector, deltaTime * m_speed);
			// attack hero
			PrepareAttack(distanceVector, deltaTime);
			m_pActiveState->Update(deltaTime);
		}

		// set lai toa do cho effect
		Vector2 oldPos = m_pActiveState->Get2DPosition();
		m_pExplosionDie->Set2DPosition(oldPos.x, oldPos.y + 15);
		m_pEffectHurt[idHero]->Set2DPosition(oldPos);
	}
}

void Enermy::Draw()
{
	if (m_timeDie > 0 && (flash + 1) % 4) {
		m_pActiveState->Draw();
		if (m_isDie)
			m_pExplosionDie->Draw();
	}
	if (m_timeHurt > -1)
		m_pEffectHurt[idHero]->Draw();
}

void Enermy::Attack()
{
	if (!Player::GetInstance()->m_isDie)
	{
		m_isAttacking = true;
		m_timeAttack = 1;
		m_listState[3]->Set2DPosition(m_pActiveState->Get2DPosition());
		m_pActiveState = m_listState[3];
		Player::GetInstance()->Hurt(m_dame);
	}
}

void Enermy::Walk()
{
	m_listState[1]->Set2DPosition(m_pActiveState->Get2DPosition());
	m_pActiveState = m_listState[1];
}

void Enermy::Idle()
{
	m_listState[0]->Set2DPosition(m_pActiveState->Get2DPosition());
	m_pActiveState = m_listState[0];
}

void Enermy::Die()
{
	m_isDie = true;
	score += (isBoss) ? 30 : 10;
	Vector2 oldPos = m_pActiveState->Get2DPosition();
	m_listState[5]->Set2DPosition(oldPos);
	m_pActiveState = m_listState[5];
	m_pExplosionDie->Set2DPosition(oldPos.x, oldPos.y + 15);
}

void Enermy::Win()
{
	m_listState[4]->Set2DPosition(m_pActiveState->Get2DPosition());
	m_pActiveState = m_listState[4];
}

void Enermy::Hurt(int dame)
{
	m_timeHurt = 1;
	m_heal -= dame;
	if (m_heal > 0)
	{
		Vector2 oldPos = m_pActiveState->Get2DPosition();
		m_pEffectHurt[idHero]->Set2DPosition(oldPos);
		m_listState[7]->Set2DPosition(oldPos);
		m_pActiveState = m_listState[7];
	}
	else
	{
		m_heal = 0;
		Die();
	}
}

void Enermy::ChaseHero(Vector2 distanceVector, float d)
{
	int distance = distanceVector.Length();
	if (distance <= m_distanceChase) {
		if (m_timeHurt == -1)
			Move(distanceVector, d);
	}
	else if (m_pActiveState == m_listState[1] || m_pActiveState == m_listState[2])
	{
		Idle();
	}
}

void Enermy::Move(Vector2 distanceVector, float d)
{
	if (distanceVector.Length() > m_distanceAttack)
	{
		Walk();
		Vector2 oldPos = m_pActiveState->Get2DPosition();
		if (abs(distanceVector.x) > abs(distanceVector.y)) {
			if (distanceVector.x > 0)
				m_pActiveState->Set2DPosition(oldPos.x + d, oldPos.y);
			else
				m_pActiveState->Set2DPosition(oldPos.x - d, oldPos.y);
		}
		else
		{
			if (distanceVector.y > 0)
				m_pActiveState->Set2DPosition(oldPos.x, oldPos.y + d);
			else
				m_pActiveState->Set2DPosition(oldPos.x, oldPos.y - d);
		}
	}
}

void Enermy::PrepareAttack(Vector2 distanceVector, float deltaTime)
{
	if (m_timeHurt == -1)
	{
		int distance = distanceVector.Length();
		if (distance < m_distanceAttack) {
			m_countToAttack -= deltaTime;
			if (m_countToAttack < 0)
			{
				Attack();
				m_countToAttack = 1;
			}
		}
		else
			m_countToAttack = 0.25;
	}
	else m_countToAttack = 0.25;
}

Vector2 Enermy::Get2DPosition()
{
	return m_pActiveState->Get2DPosition();
}
