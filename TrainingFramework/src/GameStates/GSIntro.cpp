#include "GSIntro.h"
#include "SpriteAnimation.h"
#include "GameHelper/LoadingHelper.h"
#include <future>

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSIntro::GSIntro()
{
	m_time = 0;
	m_isPause = false;
}

GSIntro::~GSIntro()
{
}

void GSIntro::Init()
{
	//std::async(&GSIntro::Loading, this);
	//Loading();

	// loading animation
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	auto texture = ResourceManagers::GetInstance()->GetTexture("loading_game");
	m_loading = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.3f);
	m_loading->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_loading->SetSize(screenWidth, screenWidth / 2);

	// logo
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("logo");
	m_logo = std::make_shared<Sprite2D>(model, shader, texture);
	m_logo->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_logo->SetSize(200, 200);
}

void GSIntro::Exit()
{
	m_isPause = true;
}

void GSIntro::Pause()
{
	m_isPause = true;
}

void GSIntro::Resume()
{
	m_isPause = false;
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

void GSIntro::HandleMouseMoveEvents(int x, int y)
{
}

void GSIntro::Update(float deltaTime)
{
	if (!m_isPause)
	{
		m_time += deltaTime;
		if (m_time > TIME_DISPLAY_LOGO)
			m_loading->Update(deltaTime);
		if (m_loading->IsLastFrame())
			GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Menu);
	}
}

void GSIntro::Draw()
{
	if (!m_isPause)
	{
		if (m_time <= TIME_DISPLAY_LOGO)
			m_logo->Draw();
		else
			m_loading->Draw();
	}
}

void GSIntro::Loading()
{
	std::shared_ptr<LoadingHelper> lh = LoadingHelper::GetInstance();
	lh->LoadingIntro();
	lh->LoadingMenu();
	lh->LoadingHeroSelecting(3, 7);
	lh->LoadingMusic();
}
