#include "GSSetting.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
extern bool isBgMusicOn;
extern bool isEfMusicOn;
extern bool isTextFloatOn;

int settingSound;
float v = 1;
Vector2 dVolume = {610, 226};

GSSetting::GSSetting()
{
	constants = Constants::GetInstance();
	v = ResourceManagers::GetInstance()->GetGlobalVolume();
}

GSSetting::~GSSetting()
{
}

void GSSetting::Init()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	// volume bar [0]
	auto texture = ResourceManagers::GetInstance()->GetTexture("volume_bar");
	auto spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->Set2DPosition(dVolume.x-(1-v)*150, dVolume.y);
	spr2d->SetSize(300, 22);
	m_listSprite2D.push_back(spr2d);

	// background [1]
	texture = ResourceManagers::GetInstance()->GetTexture("settings");
	spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->Set2DPosition(screenWidth / 2, screenHeight / 2);
	spr2d->SetSize(screenWidth, screenHeight);
	m_listSprite2D.push_back(spr2d);
	
	// button bgmusic on [0]
	texture = ResourceManagers::GetInstance()->GetTexture("button_on");
	auto button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(550, 300);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isBgMusicOn = !isBgMusicOn;
		ResourceManagers::GetInstance()->StopAllSound();
	});
	button->SetEnable(isBgMusicOn);
	m_listButton.push_back(button);

	// button efmusic on [1]
	texture = ResourceManagers::GetInstance()->GetTexture("button_on");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(550, 375);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isEfMusicOn = !isEfMusicOn;
	});
	button->SetEnable(isEfMusicOn);
	m_listButton.push_back(button);

	// button bgmusic off [2]
	texture = ResourceManagers::GetInstance()->GetTexture("button_off");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(550, 300);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isBgMusicOn = !isBgMusicOn;
		ResourceManagers::GetInstance()->PlaySound("setting", true);
	});
	button->SetEnable(!isBgMusicOn);
	m_listButton.push_back(button);

	// button efmusic off [3]
	texture = ResourceManagers::GetInstance()->GetTexture("button_off");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(550, 375);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isEfMusicOn = !isEfMusicOn;
	});
	button->SetEnable(!isEfMusicOn);
	m_listButton.push_back(button);

	// button quit [4]
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	// button 5 [5]
	texture = ResourceManagers::GetInstance()->GetTexture("button_5");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(450, 590);
	button->SetSize(50, 50);
	button->SetOnClick([]() {
		Constants::GetInstance()->NUM_ENERMY = 5;
	});
	m_listButton.push_back(button);

	// button 10 [6]
	texture = ResourceManagers::GetInstance()->GetTexture("button_10");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(550, 590);
	button->SetSize(50, 50);
	button->SetOnClick([]() {
		Constants::GetInstance()->NUM_ENERMY = 10;
	});
	m_listButton.push_back(button);

	// button 15 [7]
	texture = ResourceManagers::GetInstance()->GetTexture("button_15");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(650, 590);
	button->SetSize(50, 50);
	button->SetOnClick([]() {
		Constants::GetInstance()->NUM_ENERMY = 15;
	});
	m_listButton.push_back(button);

	// button textfloat on [8]
	texture = ResourceManagers::GetInstance()->GetTexture("button_on");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(675, 685);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isTextFloatOn = !isTextFloatOn;
	});
	button->SetEnable(isTextFloatOn);
	m_listButton.push_back(button);

	// button textfloat off [9]
	texture = ResourceManagers::GetInstance()->GetTexture("button_off");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(675, 685);
	button->SetSize(75, 41);
	button->SetOnClick([]() {
		isTextFloatOn = !isTextFloatOn;
	});
	button->SetEnable(!isTextFloatOn);
	m_listButton.push_back(button);

	if (isBgMusicOn)
		settingSound = ResourceManagers::GetInstance()->PlaySound("setting", true);
}

void GSSetting::Exit()
{
	ResourceManagers::GetInstance()->StopAllSound();
}

void GSSetting::Pause()
{
}

void GSSetting::Resume()
{
}

void GSSetting::HandleEvents()
{
}

void GSSetting::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSSetting::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto b : m_listButton)
	{
		if (b->IsEnable())
		{
			b->HandleTouchEvents(x, y, bIsPressed);
			if (b->IsHandle()) break;
		}
	}

	// xu ly volume
	if (bIsPressed)
	{
		float rx = dVolume.x - x;
		float ry = dVolume.y - y;
		if (rx >= 0 && rx <= 150 && ry >= -11 && ry <= 11)
		{
			v = 1 - rx / 150;
			ResourceManagers::GetInstance()->SetGlobalVolume(v);
			m_listSprite2D[0]->Set2DPosition(dVolume.x - rx, dVolume.y);
		}
	}
}

void GSSetting::HandleMouseMoveEvents(int x, int y)
{
}

void GSSetting::Update(float deltaTime)
{
	m_listButton[0]->SetEnable(isBgMusicOn);
	m_listButton[1]->SetEnable(isEfMusicOn);
	m_listButton[2]->SetEnable(!isBgMusicOn);
	m_listButton[3]->SetEnable(!isEfMusicOn);
	m_listButton[8]->SetEnable(isTextFloatOn);
	m_listButton[9]->SetEnable(!isTextFloatOn);
	for (int i = 5; i < 8; ++i)
	{
		if (constants->NUM_ENERMY == (i - 4) * 5)
			m_listButton[i]->SetSize(75, 75);
		else
			m_listButton[i]->SetSize(50, 50);
	}
}

void GSSetting::Draw()
{
	for (auto spr : m_listSprite2D)
	{
		spr->Draw();
	}
	for (auto b : m_listButton)
	{
		if (b->IsEnable())
		{
			b->Draw();
		}
	}
}

