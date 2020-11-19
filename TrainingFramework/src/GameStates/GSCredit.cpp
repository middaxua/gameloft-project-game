#include "GSCredit.h"


extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
extern bool isBgMusicOn;

int creditSound;

GSCredit::GSCredit()
{
	m_isPause = false;
}

GSCredit::~GSCredit()
{
}

void GSCredit::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	
	// background
	auto texture = ResourceManagers::GetInstance()->GetTexture("credit");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	//button back
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	if (isBgMusicOn)
		creditSound = ResourceManagers::GetInstance()->PlaySound("credit", true);
}

void GSCredit::Exit()
{
	m_isPause = true;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->StopAllSound();
}

void GSCredit::Pause()
{
	m_isPause = true;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->StopAllSound();
}

void GSCredit::Resume()
{
	m_isPause = false;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->PlaySound("credit", true);
}

void GSCredit::HandleEvents()
{

}

void GSCredit::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSCredit::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSCredit::HandleMouseMoveEvents(int x, int y)
{
}

void GSCredit::Update(float deltaTime)
{

}

void GSCredit::Draw()
{
	if (!m_isPause)
	{
		m_BackGround->Draw();
		for (auto it : m_listButton)
		{
			it->Draw();
		}
	}
}
