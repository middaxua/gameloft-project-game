#include "GSPlay.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "SpriteAnimation.h"
#include "GameButton.h"
#include "Player.h"
#include "Enermy.h"
#include <ctime>

extern int m_id;
extern GLfloat SCALE;
extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

std::vector<std::shared_ptr<Enermy>> m_listEnermy;
std::shared_ptr<Player> player;
int nearestEnermyId = -1;
bool isPause = false, isEnd = false;
bool keyPressed = false;
float centerMiniX, centerMiniY;
float rx[50], ry[50];

GSPlay::GSPlay()
{
	
}


GSPlay::~GSPlay()
{
	m_listEnermy.clear();
}


void GSPlay::Init()
{
	if (Player::HasInstance())
		Player::FreeInstance();
	player = Player::GetInstance();
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_game");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(MAP_WIDTH, MAP_HEIGHT);
	//mini bG
	miniW = MAP_WIDTH / SCALE;
	miniH = MAP_HEIGHT / SCALE;
	m_miniBackground = std::make_shared<Sprite2D>(model, shader, texture);
	m_miniBackground->Set2DPosition(25 + miniW / 2, 130 + miniH / 2);
	m_miniBackground->SetSize(miniW, miniH);

	// Target scope
	texture = ResourceManagers::GetInstance()->GetTexture("target_scope");
	m_targetScope = std::make_shared<Sprite2D>(model, shader, texture);
	m_targetScope->SetSize(42, 42);

	// Mini hero
	texture = ResourceManagers::GetInstance()->GetTexture("mini_hero");
	miniHero = std::make_shared<Sprite2D>(model, shader, texture);
	Vector2 k = m_BackGround->Get2DPosition();
	centerMiniX = 27.5 + 0.5 * miniW;
	centerMiniY = 132.5 + 0.5 * miniH;
	miniHero->Set2DPosition(centerMiniX, centerMiniY);
	miniHero->SetSize(5, 5);

	// Radius skill
	char r_ulti_skill[] = "ulti_skill_0";
	r_ulti_skill[strlen(r_ulti_skill) - 1] = m_id + '0';
	texture = ResourceManagers::GetInstance()->GetTexture(r_ulti_skill);
	m_radiusSkill = std::make_shared<Sprite2D>(model, shader, texture);
	m_radiusSkill->SetSize(300, 300);

	// status shift
	char status[] = "status_0";
	for (int i = 1; i <= 5; ++i)
	{
		status[strlen(status) - 1] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(status);
		m_status[i] = std::make_shared<Sprite2D>(model, shader, texture);
		m_status[i]->Set2DPosition(100, 120);
		m_status[i]->SetSize(145, 10);
	}

	//button pause
	texture = ResourceManagers::GetInstance()->GetTexture("button_pause");
	button_pause = std::make_shared<GameButton>(model, shader, texture);
	button_pause->Set2DPosition(60, 60);
	button_pause->SetSize(60, 60);
	button_pause->SetOnClick([]() {
		isPause = true;
	});

	//button play
	texture = ResourceManagers::GetInstance()->GetTexture("button_resume");
	button_play = std::make_shared<GameButton>(model, shader, texture);
	button_play->Set2DPosition(60, 60);
	button_play->SetSize(60, 60);
	button_play->SetOnClick([]() {
		isPause = false;
	});

	//button quit
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		isPause = isEnd = false;
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	//button normal skill
	char normal_skill[] = "button_skill_normal_0";
	normal_skill[strlen(normal_skill) - 1] = m_id + '0';
	texture = ResourceManagers::GetInstance()->GetTexture(normal_skill);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 130, screenHeight - 50);
	button->SetSize(50, 50);
	button->SetOnClick([]() {
		if (!isPause)
			player->Attack(nearestEnermyId);
	});
	m_listButton.push_back(button);

	//button ulti skill
	char ulti_skill[] = "button_skill_ulti_0";
	ulti_skill[strlen(ulti_skill) - 1] = m_id + '0';
	texture = ResourceManagers::GetInstance()->GetTexture(ulti_skill);
	button_ulti = std::make_shared<GameButton>(model, shader, texture);
	button_ulti->Set2DPosition(screenWidth - 55, screenHeight - 125);
	button_ulti->SetSize(50, 50);
	button_ulti->SetOnClick([]() {
	});

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));

	// enermy
	m_listEnermy.clear();
	float a, b;
	srand(time(NULL));
	for (int i = 0; i < NUM_OF_ENERMY; ++i) {
		std::shared_ptr<Enermy> enermy = std::make_shared<Enermy>(rand()%3);
		a = (rand() % MAP_WIDTH - MAP_WIDTH * 0.5);
		b = (rand() % MAP_HEIGHT - MAP_HEIGHT * 0.5);
		rx[i] = 27.5 + (0.5 + a / MAP_WIDTH) * miniW;
		ry[i] = 132.5 + (0.5 + b / MAP_HEIGHT) * miniH;
		enermy->CurrentState()->Set2DPosition(a + 240, b + 300);
		m_listEnermy.push_back(enermy);
	}

	// mini enermy
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("mini_enermy");
	for (int i = 0; i < NUM_OF_ENERMY; ++i)
	{
		miniEnermy[i] = std::make_shared<Sprite2D>(model, shader, texture);
		miniEnermy[i]->Set2DPosition(rx[i], ry[i]);
		miniEnermy[i]->SetSize(5, 5);
	}
	// health bar
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("health_bar");
	m_healthBar[0] = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0 / 11);
	m_healthBar[0]->Set2DPosition(240, 235);
	m_healthBar[0]->SetSize(100, 10);

	Vector2 d = { 0, 65 };
	for (int i = 1; i <= NUM_OF_ENERMY; ++i)
	{
		m_healthBar[i] = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0 / 11);
		m_healthBar[i]->Set2DPosition(m_listEnermy[i - 1]->CurrentState()->Get2DPosition() - d);
		m_healthBar[i]->SetSize(100, 10);
	}

	// kill cd
	texture = ResourceManagers::GetInstance()->GetTexture("kill_cd");
	for (int i = 0; i < 2; ++i)
	{
		m_killCD[i] = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0 / 11);
		m_killCD[i]->Set2DPosition(screenWidth - 130 + 75 * i, screenHeight - 50 - 75*i);
		m_killCD[i]->SetSize(50, 50);
	}

	ResourceManagers::GetInstance()->PlaySound("bgsound_play", true);
}

void GSPlay::Exit()
{
	ResourceManagers::GetInstance()->PauseSound("bgsound_play");
}


void GSPlay::Pause()
{

}

void GSPlay::Resume()
{

}


void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (!isPause)
	{
		if (player->m_isDie)
		{
			keyPressed = false;
			keyType = 0;
		}
		else
		{
			if (bIsPressed) {
				if (key == KEY_MOVE_RIGHT || key == KEY_MOVE_LEFT || key == KEY_MOVE_BACKWORD || key == KEY_MOVE_FORWORD)
				{
					keyPressed = true;
					switch (key)
					{
					case KEY_MOVE_RIGHT:
						keyType |= RIGHT;
						break;
					case KEY_MOVE_LEFT:
						keyType |= LEFT;
						break;
					case KEY_MOVE_BACKWORD:
						keyType |= DOWN;
						break;
					case KEY_MOVE_FORWORD:
						keyType |= UP;
						break;
					}
				}
				else if (key == KEY_LSHIFT)
				{
					keyType |= LSHIFT;
				}
			}
			else {
				if (key == KEY_LSHIFT)
				{
					keyType &= ~LSHIFT;
				}
				else {
					switch (key)
					{
					case KEY_MOVE_RIGHT:
						keyType &= ~RIGHT;
						break;
					case KEY_MOVE_LEFT:
						keyType &= ~LEFT;
						break;
					case KEY_MOVE_BACKWORD:
						keyType &= ~DOWN;
						break;
					case KEY_MOVE_FORWORD:
						keyType &= ~UP;
						break;
					}
					if (keyType == 0) {
						if (player->IsMove())
							player->Idle();
						keyPressed = false;
					}
				}
			}
		}
	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
	if (button_ulti->IsClicked(x, y, bIsPressed) && player->m_timeUltimate == 0)
		isUsingUlti = true;
	if (!bIsPressed && isUsingUlti)
	{
		isUsingUlti = false;
		player->Ultimate(x, y);
	}
	if (isPause) button_play->HandleTouchEvents(x, y, bIsPressed);
	else button_pause->HandleTouchEvents(x, y, bIsPressed);
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
	if (isUsingUlti)
		m_radiusSkill->Set2DPosition(x, y);
}

void GSPlay::Update(float deltaTime)
{
	if (!isPause)
	{
		m_killCD[0]->SetCurrentFrame(10 - max(0, (int)(player->m_timeAttack * 10)));
		m_killCD[1]->SetCurrentFrame(10 - (int)(player->m_timeUltimate));
		m_healthBar[0]->SetCurrentFrame(player->m_heal / 10);
		for (int i = 1; i <= NUM_OF_ENERMY; ++i)
		{
			m_healthBar[i]->SetCurrentFrame(m_listEnermy[i - 1]->m_heal / 10);
		}
		player->Update(deltaTime);
		if ((keyType & LSHIFT) == 0 && player->m_timeTired < 5)
		{
			player->m_timeTired += deltaTime;
			if (player->m_timeTired > 5)
			{
				player->m_timeTired = 5;
				player->m_isTired = false;
			}
		}

		// xu ly nhan vat di chuyen
		if (!player->m_isAttacking)
		{
			if (keyPressed) {
				if (keyType & LSHIFT && !player->m_isTired)
				{
					player->Run();
					MoveCharacter(m_run_speed * deltaTime);
					player->m_timeTired -= deltaTime;
					if (player->m_timeTired < 0)
					{
						player->m_timeTired = 0;
						player->m_isTired = true;
					}
				}
				else
				{
					player->Walk();
					MoveCharacter(m_walk_speed * deltaTime);
				}
			}
			// Tim enermy gan nhat
			if (!player->m_isDie)
			{
				FindNearestEnermy();
				if (nearestEnermyId != -1) {
					m_targetScope->Set2DPosition(m_listEnermy[nearestEnermyId]->CurrentState()->Get2DPosition());
				}
			}
		}
		for (auto obj : m_listEnermy)
		{
			obj->Update(deltaTime);
		}
	}
	if (isEnd)
	{
		for (auto obj : m_listEnermy)
		{
			obj->Update(deltaTime);
		}
	}
}

void GSPlay::Draw()
{
	m_BackGround->Draw();
	if (player->m_timeDisplayEffectUltimate > 0)
	{
		player->m_effectUltimate->Draw();
	}
	//// draw rocks
	//for (auto it : m_listSprite2D)
	//{
	//	it->Draw();
	//}
	for (auto obj : m_listEnermy)
	{
		if (obj->CurrentState()->Get2DPosition().y < player->CurrentState()->Get2DPosition().y)
			obj->Draw();
	}
	player->Draw();
	for (auto obj : m_listEnermy)
	{
		if (obj->CurrentState()->Get2DPosition().y >= player->CurrentState()->Get2DPosition().y)
			obj->Draw();
	}
	if (!isEnd && nearestEnermyId != -1)
		m_targetScope->Draw();
	if (isUsingUlti)
		m_radiusSkill->Draw();
	if (!player->m_isDie)
		m_healthBar[0]->Draw();
	for (int i = 1; i <= NUM_OF_ENERMY; ++i)
	{
		if (!m_listEnermy[i - 1]->m_isDie)
			m_healthBar[i]->Draw();
	}
	m_miniBackground->Draw();
	for (int i = 0; i < NUM_OF_ENERMY; ++i) {
		if (!m_listEnermy[i]->m_isDie)
			miniEnermy[i]->Draw();
	}
	miniHero->Draw();
	//draw status
	if (player->m_timeTired > 1)
		m_status[(int)player->m_timeTired]->Draw();
	// draw button pause and resume
	if (isPause) button_play->Draw();
	else button_pause->Draw();
	// draw buttons
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_score->Draw();
	button_ulti->Draw();
	m_killCD[0]->Draw();
	m_killCD[1]->Draw();
}

void GSPlay::MoveEffect(std::shared_ptr<SpriteAnimation> effect, int distance)
{
	Vector2 oldPos = effect->Get2DPosition();
	Vector2 newPos = oldPos;
	if (keyType & LEFT)
	{
		newPos.x = oldPos.x + distance;
		if (MAP_WIDTH / 2 - newPos.x < 0)
			newPos.x = MAP_WIDTH / 2;
	}
	else if (keyType & RIGHT)
	{
		newPos.x = oldPos.x - distance;
		if (MAP_WIDTH / 2 + newPos.x < screenWidth)
			newPos.x = screenWidth - MAP_WIDTH / 2;
	}
	if (keyType & DOWN)
	{
		newPos.y = oldPos.y - distance;
		if (MAP_HEIGHT / 2 + newPos.y < screenHeight)
			newPos.y = screenHeight - MAP_HEIGHT / 2;
	}
	else if (keyType & UP)
	{
		newPos.y = oldPos.y + distance;
		if (MAP_HEIGHT / 2 - newPos.y < 0)
			newPos.y = MAP_HEIGHT / 2;
	}
	effect->Set2DPosition(newPos);
}

void GSPlay::MoveEnermy(int distance)
{
	if (keyType & LEFT)
	{
		for (int i = 0; i < m_listEnermy.size(); ++i)
		{
			Vector2 oldPos = m_listEnermy[i]->CurrentState()->Get2DPosition();
			Vector2 newPos = oldPos;
			newPos.x = oldPos.x + distance;
			if (MAP_WIDTH / 2 - newPos.x < 0)
				newPos.x = MAP_WIDTH / 2;
			m_listEnermy[i]->CurrentState()->Set2DPosition(newPos);
			Vector2 d = { 0, 65 };
			m_healthBar[i + 1]->Set2DPosition(m_listEnermy[i]->CurrentState()->Get2DPosition() - d);
		}
	}
	else if (keyType & RIGHT)
	{
		for (int i = 0; i < m_listEnermy.size(); ++i)
		{
			Vector2 oldPos = m_listEnermy[i]->CurrentState()->Get2DPosition();
			Vector2 newPos = oldPos;
			newPos.x = oldPos.x - distance;
			if (MAP_WIDTH / 2 + newPos.x < screenWidth)
				newPos.x = screenWidth - MAP_WIDTH / 2;
			m_listEnermy[i]->CurrentState()->Set2DPosition(newPos);
			Vector2 d = { 0, 65 };
			m_healthBar[i + 1]->Set2DPosition(m_listEnermy[i]->CurrentState()->Get2DPosition() - d);
		}
	}
	if (keyType & DOWN)
	{
		for (int i = 0; i < m_listEnermy.size(); ++i)
		{
			Vector2 oldPos = m_listEnermy[i]->CurrentState()->Get2DPosition();
			Vector2 newPos = oldPos;
			newPos.y = oldPos.y - distance;
			if (MAP_HEIGHT / 2 + newPos.y < screenHeight)
				newPos.y = screenHeight - MAP_HEIGHT / 2;
			m_listEnermy[i]->CurrentState()->Set2DPosition(newPos);
			Vector2 d = { 0, 65 };
			m_healthBar[i + 1]->Set2DPosition(m_listEnermy[i]->CurrentState()->Get2DPosition() - d);
		}
	}
	else if (keyType & UP)
	{
		for (int i = 0; i < m_listEnermy.size(); ++i)
		{
			Vector2 oldPos = m_listEnermy[i]->CurrentState()->Get2DPosition();
			Vector2 newPos = oldPos;
			newPos.y = oldPos.y + distance;
			if (MAP_HEIGHT / 2 - newPos.y < 0)
				newPos.y = MAP_HEIGHT / 2;
			m_listEnermy[i]->CurrentState()->Set2DPosition(newPos);
			Vector2 d = { 0, 65 };
			m_healthBar[i +	1]->Set2DPosition(m_listEnermy[i]->CurrentState()->Get2DPosition() - d);
		}
	}
}

void GSPlay::MoveMap(int distance)
{
	Vector2 oldPos = m_BackGround->Get2DPosition();
	Vector2 newPos = oldPos;
	if (keyType & LEFT)
	{
		newPos.x = oldPos.x + distance;
		if (MAP_WIDTH / 2 - newPos.x < 0)
			newPos.x = MAP_WIDTH / 2;
	}
	else if (keyType & RIGHT)
	{
		newPos.x = oldPos.x - distance;
		if (MAP_WIDTH / 2 + newPos.x < screenWidth)
			newPos.x = screenWidth - MAP_WIDTH / 2;
	}
	if (keyType & DOWN)
	{
		newPos.y = oldPos.y - distance;
		if (MAP_HEIGHT / 2 + newPos.y < screenHeight)
			newPos.y = screenHeight - MAP_HEIGHT / 2;
	}
	else if (keyType & UP)
	{
		newPos.y = oldPos.y + distance;
		if (MAP_HEIGHT / 2 - newPos.y < 0)
			newPos.y = MAP_HEIGHT / 2;
	}
	m_BackGround->Set2DPosition(newPos);
	float x = miniW * (screenWidth * 0.5 - newPos.x) / MAP_WIDTH;
	float y = miniH * (screenHeight * 0.5 - newPos.y) / MAP_HEIGHT;
	miniHero->Set2DPosition(centerMiniX + x, centerMiniY + y);
	/*for (int i = 0; i < NUM_OF_ENERMY; ++i) {
		miniEnermy[i]->Set2DPosition(rx[i] - x, ry[i] - y);
	}*/
}

void GSPlay::MoveCharacter(int distance)
{
	MoveMap(distance);
	MoveEnermy(distance);
	MoveEffect(player->m_effectUltimate, distance);
}

void GSPlay::SetNewPostionForBullet()
{
}

void GSPlay::FindNearestEnermy()
{
	nearestEnermyId = -1;
	int minDistance = player->m_distanceAttack[m_id];
	Vector2 heroPos = player->CurrentState()->Get2DPosition();
	for (int i = 0; i < m_listEnermy.size(); ++i) {
		if (!m_listEnermy[i]->m_isDie) {
			Vector2 enermyPos = m_listEnermy[i]->CurrentState()->Get2DPosition();
			int distance = (heroPos - enermyPos).Length();
			if (distance < minDistance) {
				minDistance = distance;
				nearestEnermyId = i;
			}
		}
	}
}
