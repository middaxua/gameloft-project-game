#include "Player.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"
#include "Enermy.h"

extern int m_id;
extern int screenWidth;
extern int screenHeight;
extern GLfloat SCALE;
extern bool isPause, isEnd;
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

	arrayCharacterName = new char*[3];
	arrayCharacterName[0] = new char[15]{ "archer_0" };
	arrayCharacterName[1] = new char[15]{ "icewizard_0" };
	arrayCharacterName[2] = new char[15]{ "knight_0" };

	std::shared_ptr<SpriteAnimation> c = nullptr;
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	std::shared_ptr<Texture> texture = nullptr;
	std::shared_ptr<SpriteAnimation> tmpCharacter = nullptr;
	int len = strlen(arrayCharacterName[m_id]);
	for (int i = 0; i < 8; ++i) {
		arrayCharacterName[m_id][len - 1] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(arrayCharacterName[m_id]);
		tmpCharacter = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
		tmpCharacter->Set2DPosition(240, 300);
		tmpCharacter->SetSize(128, 128);
		m_listState.push_back(tmpCharacter);
	}
	m_pActiveState = m_listState[0];

	// animation explosion attack
	int numberFrame[3] = { 25,36,5 };
	int size[3][2] = { {96, 96}, {128, 128}, {77, 77} };
	arrayEffectAttackName = new char[20]{ "effect_attack_0" };
	for (int i = 0; i < 3; ++i) {
		arrayEffectAttackName[14] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(arrayEffectAttackName);
		m_pExplosion[i] = std::make_shared<SpriteAnimation>(model, shader, texture, numberFrame[i], 1.0 / numberFrame[i]);
		m_pExplosion[i]->SetSize(size[i][0], size[i][1]);
	}

	// effect ultimate
	char effect_ultimate[] = "effect_ulti_0";
	effect_ultimate[strlen(effect_ultimate) - 1] = m_id + '0';
	texture = ResourceManagers::GetInstance()->GetTexture(effect_ultimate);
	m_effectUltimate = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 1.0 / 4);
	m_effectUltimate->SetSize(300, 300);

	// Arrow
	if (m_id == 0)
	{
		shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		char arrow_id[] = "arrow_0";
		int lenArrowId = strlen(arrow_id);
		for (int i = 0; i < 4; ++i)
		{
			arrow_id[lenArrowId - 1] = i + '0';
			texture = ResourceManagers::GetInstance()->GetTexture(arrow_id);
			m_arrow[i] = std::make_shared<Sprite2D>(model, shader, texture);
			if (i % 2 == 0)
				m_arrow[i]->SetSize(45, 10);
			else
				m_arrow[i]->SetSize(10, 45);
		}
	}
}

Player::~Player() {
	for (int i = 0; i < 3; ++i)
	{
		delete[] arrayCharacterName[i];
	}
	delete[] arrayCharacterName;
	delete[] arrayEffectAttackName;
}

void Player::Init()
{
}

void Player::Exit()
{
}


void Player::Pause()
{

}

void Player::Resume()
{

}


void Player::HandleEvents()
{

}

void Player::HandleKeyEvents(int key, bool bIsPressed)
{

}

void Player::HandleTouchEvents(int x, int y, bool bIsPressed)
{

}

void Player::HandleMouseMoveEvents(int x, int y)
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
			m_pActiveState = m_listState[0];
		}
		else
		{
			m_pExplosion[m_id]->Update(deltaTime);
			if (m_id == 0)
				m_arrow[m_arrowId]->Set2DPosition(m_arrow[m_arrowId]->Get2DPosition() + distanceHeroEnermy * deltaTime);
			m_timeAttack -= deltaTime;
		}
	}
	if (m_timeHurt < 0)
	{
		if (m_timeHurt != -1)
		{
			m_timeHurt = -1;
			m_pActiveState = m_listState[0];
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
			m_pExplosion[m_id]->Draw();
			if (m_id == 0)
				m_arrow[m_arrowId]->Draw();
		}
	}
}

void Player::Attack(int id)
{
	if (!m_isAttacking) {
		ResourceManagers::GetInstance()->PlaySound("attack");
		m_isAttacking = true;
		m_timeAttack = 1;
		m_pActiveState = m_listState[3];
		m_pActiveState->SetCurrentFrame(0);
		m_pExplosion[m_id]->Set2DPosition(m_pActiveState->Get2DPosition() + posAttackEffect[m_id]);
		if (id > -1) {
			if (m_id == 0)
			{
				// caculate tan enermy and hero
				Vector2 heroPos = m_pActiveState->Get2DPosition();
				Vector2 enermyPos = m_listEnermy[id]->CurrentState()->Get2DPosition();
				float tan = 999999;
				if (enermyPos.x - heroPos.x != 0)
					tan = (enermyPos.y - heroPos.y) / (enermyPos.x - heroPos.x);
				distanceHeroEnermy = enermyPos - heroPos;
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
			m_listEnermy[id]->Hurt(m_dame[m_id]);
		}
	}
}

void Player::Run()
{
	m_pActiveState = m_listState[2];
}

void Player::Walk()
{
	m_pActiveState = m_listState[1];
}

void Player::Idle()
{
	m_pActiveState = m_listState[0];
}

void Player::Die()
{
	ResourceManagers::GetInstance()->PlaySound("die");
	m_isDie = true;
	m_pActiveState = m_listState[5];
}

void Player::Hurt(int dame)
{
	ResourceManagers::GetInstance()->PlaySound("hurt");
	m_heal -= dame;
	if (m_heal > 0)
		m_pActiveState = m_listState[7];
	else
	{
		m_heal = 0;
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

void Player::Ultimate(float x, float y)
{
	ResourceManagers::GetInstance()->PlaySound("ultimate");
	m_timeUltimate = 10;
	m_effectUltimate->SetCurrentFrame(0);
	m_effectUltimate->Set2DPosition(x, y);
	m_timeDisplayEffectUltimate = 1;
	Vector2 mousePos = { x, y };
	for (auto enermy : m_listEnermy)
	{
		Vector2 enermyPos = enermy->CurrentState()->Get2DPosition();
		if (!enermy->m_isDie)
		{
			int distance = (enermyPos - mousePos).Length();
			if (distance < 150)
			{
				enermy->Hurt(m_dame[m_id]);
			}
		}
	}
}

bool Player::IsMove()
{
	return m_pActiveState == m_listState[1] || m_pActiveState == m_listState[2];
}
