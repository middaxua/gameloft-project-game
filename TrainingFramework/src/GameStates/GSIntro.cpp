#include "GSIntro.h"
#include "SpriteAnimation.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSIntro::GSIntro()
{
	m_time = 0;
}


GSIntro::~GSIntro()
{
}


void GSIntro::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	auto texture = ResourceManagers::GetInstance()->GetTexture("loading_game");
	m_loading = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.3f);
	m_loading->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_loading->SetSize(480, 240);

	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("logo");
	m_logo = std::make_shared<Sprite2D>(model, shader, texture);
	m_logo->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_logo->SetSize(200, 200);
}

void GSIntro::Exit()
{
	
}


void GSIntro::Pause()
{
	
}

void GSIntro::Resume()
{
	
}


void GSIntro::HandleEvents()
{
	
}

void GSIntro::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSIntro::HandleTouchEvents(int x, int y, bool bIsPressed)
{

}

void GSIntro::Update(float deltaTime)
{
	m_time += deltaTime;
	if (m_time > 6.3)
	{
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
		//m_time = 0;
	}
	else if (m_time > 2.2 && m_time < 5.2)
	{
		m_loading->Update(deltaTime);
	}
}

void GSIntro::Draw()
{
	if (m_time <= 2.3)
		m_logo->Draw();
	else
		m_loading->Draw();
}
