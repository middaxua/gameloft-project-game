#include "GSHeroSelecting.h"
#include "SpriteAnimation.h"
#include "GameButton.h"
#include "Sprite2D.h"
#include "Text.h"

int m_id;
extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine

GSHeroSelecting::GSHeroSelecting()
{
	m_time = 0.0;
	m_id = 0;

	arrayCharacterName = new char*[3];
	arrayCharacterName[0] = new char[15]{ "archer_0" };
	arrayCharacterName[1] = new char[15]{ "icewizard_0" };
	arrayCharacterName[2] = new char[15]{ "knight_0" };
}


GSHeroSelecting::~GSHeroSelecting()
{
	for (int i = 0; i < 3; ++i)
	{
		delete[] arrayCharacterName[i];
	}
	delete[] arrayCharacterName;
}


void GSHeroSelecting::Init()
{
	//BackGround
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_paralel_menu");
	m_BackGround1 = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround1->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_BackGround1->SetSize(screenWidth, screenHeight);

	m_BackGround2 = std::make_shared<Sprite2D>(model, shader, texture);
	m_BackGround2->Set2DPosition(screenWidth + screenWidth / 2, screenHeight / 2);
	m_BackGround2->SetSize(screenWidth, screenHeight);

	// add 3 hero

	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	std::shared_ptr<SpriteAnimation> tmpCharacter = nullptr;
	for (int i = 0; i < 3; ++i) {
		int len = strlen(arrayCharacterName[i]);
		for (int j = 0; j < 8; ++j) {
			arrayCharacterName[i][len - 1] = j + '0';
			texture = ResourceManagers::GetInstance()->GetTexture(arrayCharacterName[i]);
			tmpCharacter = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
			tmpCharacter->Set2DPosition(i * 160 + 86, 350);
			tmpCharacter->SetSize(136, 136);
			m_SpriteAnimation[i].push_back(tmpCharacter);
		}
	}
	// 3 button select hero transparent
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("transparent");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(0 * 160 + 86, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		m_id = 0;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(1 * 160 + 86, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		m_id = 1;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(2 * 160 + 86, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		m_id = 2;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	//exit button
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	// title
	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_Text_gameName = std::make_shared<Text>(shader, font, "SELECT ONE HERO", TEXT_COLOR::RED, 1.25);
	m_Text_gameName->Set2DPosition(Vector2(screenWidth / 2 - 130, 150));

	ResourceManagers::GetInstance()->PlaySound("select_hero");
}

void GSHeroSelecting::Exit()
{
	ResourceManagers::GetInstance()->PauseSound("select_hero");
}


void GSHeroSelecting::Pause()
{
	ResourceManagers::GetInstance()->PauseSound("select_hero");
}

void GSHeroSelecting::Resume()
{
	ResourceManagers::GetInstance()->PlaySound("select_hero");
}


void GSHeroSelecting::HandleEvents()
{

}

void GSHeroSelecting::HandleKeyEvents(int key, bool bIsPressed)
{

}

void GSHeroSelecting::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto it : m_listButton)
	{
		(it)->HandleTouchEvents(x, y, bIsPressed);
		if ((it)->IsHandle()) break;
	}
}

void GSHeroSelecting::HandleMouseMoveEvents(int x, int y)
{
}

void GSHeroSelecting::Update(float deltaTime)
{
	// paralel background
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

	// hero anim
	m_time += deltaTime;
	if (m_time > 3) {
		m_id = (m_id + 1) % 8;
		m_time -= 3;
	}
	for (auto obj : m_SpriteAnimation)
	{
		obj[m_id]->Update(deltaTime);
	}
}

void GSHeroSelecting::Draw()
{
	m_BackGround1->Draw();
	m_BackGround2->Draw();
	for (auto obj : m_listButton)
	{
		obj->Draw();
	}
	for (auto obj : m_SpriteAnimation)
	{
		obj[m_id]->Draw();
	}
	m_Text_gameName->Draw();
}
