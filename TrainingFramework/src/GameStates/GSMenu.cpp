#include "GSMenu.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSMenu::GSMenu()
{

}


GSMenu::~GSMenu()
{
}



void GSMenu::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_main_menu");

	//BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround->SetSize(screenWidth, screenHeight);

	//play button
	texture = ResourceManagers::GetInstance()->GetTexture("button_play_2");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, screenHeight / 2);
	button->SetSize(250, 105);
	button->SetOnClick([]() {
		//GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
		});
	m_listButton.push_back(button);

	//music button
	texture = ResourceManagers::GetInstance()->GetTexture("button_music");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 - 135, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		
	});
	m_listButton.push_back(button);

	//setting button
	texture = ResourceManagers::GetInstance()->GetTexture("button_setting");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 - 45, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {

	});
	m_listButton.push_back(button);

	//credit button
	texture = ResourceManagers::GetInstance()->GetTexture("button_setting");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 + 45, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Credit);
	});
	m_listButton.push_back(button);

	//exit button
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 + 135, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		exit(0);
		});
	m_listButton.push_back(button);


	//text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared< Text>(shader, font, "HERO AND VILLAINS", TEXT_COLOR::GREEN, 1.25);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 120, 120));
}

void GSMenu::Exit()
{
}


void GSMenu::Pause()
{

}

void GSMenu::Resume()
{

}


void GSMenu::HandleEvents()
{

}

void GSMenu::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSMenu::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSMenu::Update(float deltaTime)
{
	m_BackGround->Update(deltaTime);
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
}

void GSMenu::Draw()
{
	m_BackGround->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}
	m_Text_gameName->Draw();
}
