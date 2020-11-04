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
#include "CharacterState.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{

}


void GSPlay::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_game");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(MAP_WIDTH, MAP_HEIGHT);

	//button back
	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(200, 100);
	button->SetSize(200, 50);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	//button skill
	texture = ResourceManagers::GetInstance()->GetTexture("button_archer_skill");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(350, 550);
	button->SetSize(75, 75);
	button->SetOnClick([]() {
		CharacterState::GetInstance(2)->ChangeState(CharacterStateTypes::CHARACTER_STATE_Attack);
	});
	m_listButton.push_back(button);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));

	// Animation
	m_hero = CharacterState::GetInstance(2);
}

void GSPlay::Exit()
{

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
	if (bIsPressed) {
		Vector2 oldPos = m_BackGround->Get2DPosition();
		if (key == KEY_RIGHT) {
			m_hero->ChangeState(CharacterStateTypes::CHARACTER_STATE_Walk);
			int newPosX = oldPos.x - 8;
			if (MAP_WIDTH / 2 + newPosX < screenWidth) {
				newPosX = screenWidth - MAP_WIDTH / 2;
				/*Vector2 oldHeroPos = m_hero->Get2DPosition();
				m_hero->Set2DPosition(oldHeroPos.x + 16, oldHeroPos.y);*/
			}
			m_BackGround->Set2DPosition(newPosX, oldPos.y);
		}
		else if (key == KEY_LEFT) {
			m_hero->ChangeState(CharacterStateTypes::CHARACTER_STATE_Walk);
			int newPosX = oldPos.x + 8;
			if (MAP_WIDTH / 2 - newPosX < 0)
				newPosX = MAP_WIDTH / 2;
			m_BackGround->Set2DPosition(newPosX, oldPos.y);
		}
		else if (key == KEY_DOWN) {
			m_hero->ChangeState(CharacterStateTypes::CHARACTER_STATE_Walk);
			int newPosY = oldPos.y - 8;
			if (MAP_HEIGHT / 2 + newPosY < screenHeight)
				newPosY = screenHeight - MAP_HEIGHT / 2;
			m_BackGround->Set2DPosition(oldPos.x, newPosY);
		}
		else if (key == KEY_UP) {
			m_hero->ChangeState(CharacterStateTypes::CHARACTER_STATE_Walk);
			int newPosY = oldPos.y + 8;
			if (MAP_HEIGHT / 2 - newPosY < 0)
				newPosY = MAP_HEIGHT / 2;
			m_BackGround->Set2DPosition(oldPos.x, newPosY);
		}
	}
	else {
		m_hero->ChangeState(CharacterStateTypes::CHARACTER_STATE_Idle);
	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSPlay::Update(float deltaTime)
{
	m_hero->Update(deltaTime);
	for (auto obj : m_listSpriteAnimations)
	{
		obj->Update(deltaTime);
	}
}

void GSPlay::Draw()
{
	m_BackGround->Draw();
	// draw buttons
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	// draw rocks
	for (auto it : m_listSprite2D)
	{
		it->Draw();
	}
	// draw anim
	for (auto obj : m_listSpriteAnimations)
	{
		obj->Draw();
	}
	m_hero->Draw();
	m_score->Draw();
}

void GSPlay::SetNewPostionForBullet()
{
}