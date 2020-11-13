#include "GSHeroSelecting.h"
#include "SpriteAnimation.h"
#include "GameButton.h"
#include "Sprite2D.h"
#include "Text.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
extern bool isMusicOn;

int idHero;
int NUM_HEROES = 3;
int NUM_STATES = 7;

GSHeroSelecting::GSHeroSelecting()
{
	m_isPause = false;
}

GSHeroSelecting::~GSHeroSelecting()
{
}

void GSHeroSelecting::Init()
{
	// BackGround
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
	std::string listHeroStates[7] = { "0", "1", "2", "3", "4", "5", "7" };
	std::string listHeroNames[3] = { "archer_", "icewizard_", "knight_" };
	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	for (int i = 0; i < NUM_HEROES; ++i) {
		for (int j = 0; j < NUM_STATES; ++j) {
			std::string fileName = listHeroNames[i] + listHeroStates[j] + "_0";
			texture = ResourceManagers::GetInstance()->GetTexture(fileName);
			std::shared_ptr<SpriteAnimation> hero = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
			hero->Set2DPosition((i + 1) * (screenWidth - 3 * 136) / 4 + 68 + i * 136, 350);
			hero->SetSize(136, 136);
			m_SpriteAnimation[i].push_back(hero);
		}
	}

	// 3 button select hero transparent
	shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	texture = ResourceManagers::GetInstance()->GetTexture("transparent");
	std::shared_ptr<GameButton> button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(1 * (screenWidth - 3 * 136) / 4 + 68 + 0 * 136, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		idHero = 0;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->PushState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(2 * (screenWidth - 3 * 136) / 4 + 68 + 1 * 136, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		idHero = 1;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(3 * (screenWidth - 3 * 136) / 4 + 68 + 2 * 136, 350);
	button->SetSize(136, 136);
	button->SetOnClick([]() {
		idHero = 2;
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->ChangeState(StateTypes::STATE_Play);
	});
	m_listButton.push_back(button);

	// exit button
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

	if (isMusicOn)
		ResourceManagers::GetInstance()->PlaySound("select_hero");
}

void GSHeroSelecting::Exit()
{
	m_isPause = true;
	if (isMusicOn)
		ResourceManagers::GetInstance()->PauseSound("select_hero");
}

void GSHeroSelecting::Pause()
{
	m_isPause = true;
	if (isMusicOn)
		ResourceManagers::GetInstance()->PauseSound("select_hero");
}

void GSHeroSelecting::Resume()
{
	m_isPause = false;
	if (isMusicOn)
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
	if (!m_isPause)
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
		if (m_SpriteAnimation[0][idHero]->IsLastFrame())
		{
			for (auto hero : m_SpriteAnimation)
			{
				hero[idHero]->ResetCurrentTime();
				hero[idHero]->SetCurrentFrame(0);
			}
			idHero = (idHero + 1) % NUM_STATES;
		}
		for (auto hero : m_SpriteAnimation)
		{
			hero[idHero]->Update(deltaTime);
		}
	}
}

void GSHeroSelecting::Draw()
{
	if (!m_isPause)
	{
		m_BackGround1->Draw();
		m_BackGround2->Draw();
		for (auto obj : m_listButton)
		{
			obj->Draw();
		}
		for (auto obj : m_SpriteAnimation)
		{
			obj[idHero]->Draw();
		}
		m_Text_gameName->Draw();
	}
}
