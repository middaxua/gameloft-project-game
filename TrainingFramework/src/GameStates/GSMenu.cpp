#include "GSMenu.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

bool isBgMusicOn = true;
bool isEfMusicOn = true;
bool isTextFloatOn = false;
int menuSound;

GSMenu::GSMenu()
{
	m_isPause = false;
}

GSMenu::~GSMenu()
{
}

void GSMenu::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_paralel_menu");

	// BackGround
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_BackGround1 = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround1->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround1->SetSize(screenWidth, screenHeight);

	m_BackGround2 = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround2->Set2DPosition(screenWidth + screenWidth / 2, screenHeight / 2);
	m_BackGround2->SetSize(screenWidth, screenHeight);

	// play button
	texture = ResourceManagers::GetInstance()->GetTexture("button_play_2");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2, screenHeight / 2);
	button->SetSize(250, 105);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PushState(StateTypes::STATE_HeroSelecting);
	});
	m_listButton.push_back(button);

	// music button
	texture = ResourceManagers::GetInstance()->GetTexture("button_music");
	m_musicOnButton = std::make_shared<GameButton>(model, shader, texture);
	m_musicOnButton->Set2DPosition(screenWidth / 2 - 135, screenHeight - 60);
	m_musicOnButton->SetSize(60, 60);
	m_musicOnButton->SetOnClick([]() {
		ResourceManagers::GetInstance()->StopAllSound();
		isBgMusicOn = false;
		isEfMusicOn = false;
	});

	// mute music button
	texture = ResourceManagers::GetInstance()->GetTexture("button_mute");
	m_musicOffButton = std::make_shared<GameButton>(model, shader, texture);
	m_musicOffButton->Set2DPosition(screenWidth / 2 - 135, screenHeight - 60);
	m_musicOffButton->SetSize(60, 60);
	m_musicOffButton->SetOnClick([]() {
		menuSound = ResourceManagers::GetInstance()->PlaySound("menu");
		isBgMusicOn = true;
		isEfMusicOn = true;
	});

	// setting button
	texture = ResourceManagers::GetInstance()->GetTexture("button_setting");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 - 45, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PushState(StateTypes::STATE_Setting);
	});
	m_listButton.push_back(button);

	// credit button
	texture = ResourceManagers::GetInstance()->GetTexture("button_credit");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 + 45, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PushState(StateTypes::STATE_Credit);
	});
	m_listButton.push_back(button);

	// exit button
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth / 2 + 135, screenHeight - 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->Cleanup();
		exit(0);
	});
	m_listButton.push_back(button);

	// sprite title game
	texture = ResourceManagers::GetInstance()->GetTexture("title_game");
	m_titleGame = std::make_shared<Sprite2D>(model, shader, texture);
	m_titleGame->Set2DPosition(screenWidth / 2, 120);
	m_titleGame->SetSize(500, 250);

	// text game title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared< Text>(shader, font, "", TEXT_COLOR::GREEN, 1.25);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 130, 120));

	menuSound = ResourceManagers::GetInstance()->PlaySound("menu", true);
}

void GSMenu::Exit()
{
	m_isPause = true;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->StopAllSound();
}

void GSMenu::Pause()
{
	m_isPause = true;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->StopAllSound();
}

void GSMenu::Resume()
{
	m_isPause = false;
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->PlaySound("menu", true);
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
	if (isBgMusicOn)
		m_musicOnButton->HandleTouchEvents(x, y, bIsPressed);
	else
		m_musicOffButton->HandleTouchEvents(x, y, bIsPressed);
}

void GSMenu::HandleMouseMoveEvents(int x, int y)
{
}

void GSMenu::Update(float deltaTime)
{
	if (!m_isPause)
	{
		Vector2 oldBG1 = m_BackGround1->Get2DPosition();
		Vector2 oldBG2 = m_BackGround2->Get2DPosition();
		float dx = 80 * deltaTime;
		if (oldBG1.x - dx < -screenWidth / 2)
		{
			m_BackGround2->Set2DPosition(oldBG2.x - dx, oldBG2.y);
			m_BackGround1->Set2DPosition(oldBG2.x - dx + screenWidth, oldBG1.y);
		}
		else if (oldBG2.x - dx < -screenWidth / 2)
		{
			m_BackGround1->Set2DPosition(oldBG1.x - dx, oldBG1.y);
			m_BackGround2->Set2DPosition(oldBG1.x - dx + screenWidth, oldBG2.y);
		}
		else
		{
			m_BackGround2->Set2DPosition(oldBG2.x - dx, oldBG2.y);
			m_BackGround1->Set2DPosition(oldBG1.x - dx, oldBG1.y);
		}
	}
}

void GSMenu::Draw()
{
	if (!m_isPause)
	{
		m_BackGround1->Draw();
		m_BackGround2->Draw();
		for (auto it : m_listButton)
		{
			it->Draw();
		}
		if (isBgMusicOn)
			m_musicOnButton->Draw();
		else
			m_musicOffButton->Draw();
		m_titleGame->Draw();
		m_Text_gameName->Draw();
	}
}
