#include "GSPlay.h"

#include "Shaders.h"
#include "Texture.h"
#include "Models.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"

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
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	//button back
	texture = ResourceManagers::GetInstance()->GetTexture("button_back");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(200, 100);
	button->SetSize(200, 50);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
	});
	m_listButton.push_back(button);

	//object rock
	texture = ResourceManagers::GetInstance()->GetTexture("Rock");
	std::shared_ptr<Sprite2D> rock = std::make_shared<Sprite2D>(model, shader, texture);
	rock->Set2DPosition(200, 300);
	rock->SetSize(50, 50);
	m_listObject.push_back(rock);

	//object grass
	texture = ResourceManagers::GetInstance()->GetTexture("Grass");
	std::shared_ptr<Sprite2D> grass = std::make_shared<Sprite2D>(model, shader, texture);
	grass->Set2DPosition(200, 400);
	grass->SetSize(50, 50);
	m_listObject.push_back(grass);

	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TEXT_COLOR::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));
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
	for (auto it : m_listObject)
	{
		it->Draw();
	}
	m_score->Draw();
}

void GSPlay::SetNewPostionForBullet()
{
}