#include "GSPlay.h"

extern int screenWidth; //need get on Graphic engine
extern int screenHeight; //need get on Graphic engine
extern bool isBgMusicOn;
extern bool isEfMusicOn;
extern bool isTextFloatOn;
extern int idHero;

int attackSound, hurtSound, dieSound;
int playSound, ultimateSound, endSound;
int score;
std::shared_ptr<Player> m_player;
std::shared_ptr<Enermy> enermy;
std::shared_ptr<Sprite2D> m_background;

GSPlay::GSPlay()
{
	constants = Constants::GetInstance();
	m_keyPressed = m_isUltimate = false;
	m_isPause = m_isEnd = false;
	m_keyType = score = 0;
	m_time = 0;
	m_mapSize = { constants->mMAP_WIDTH, constants->mMAP_HEIGHT };
	m_miniMapSize = m_mapSize / constants->mSCALE;
}

GSPlay::~GSPlay() {}

void GSPlay::Init()
{
	// background
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_game");
	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_background->SetSize(m_mapSize);
	// init player and enermy
	m_player = std::make_shared<Player>();
	for (int i = 0; i < constants->NUM_ENERMY; ++i)
	{
		m_listEnermy.push_back(std::make_shared<Enermy>());
	}
	
	// init sprite 2d
	InitSprite2Ds();
	InitGameButtons();
	InitSpriteAnim();
	InitText();

	if (isBgMusicOn)
		playSound = ResourceManagers::GetInstance()->PlaySound("bgsound_play", true);
}

void GSPlay::Exit()
{
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->StopAllSound();
}

void GSPlay::Pause()
{
	m_isPause = true;
	m_listButton[0]->SetEnable(false);
	m_listButton[1]->SetEnable(true);
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->PauseAllSound(true);
}

void GSPlay::Resume()
{
	m_isPause = false;
	m_listButton[0]->SetEnable(true);
	m_listButton[1]->SetEnable(false);
	if (isBgMusicOn)
		ResourceManagers::GetInstance()->PauseSound(playSound, false);
}

void GSPlay::HandleEvents()
{

}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
	if (!m_isPause && !m_player->IsDie())
	{
		if (bIsPressed)
		{
			if (key == KEY_MOVE_RIGHT || key == KEY_MOVE_LEFT || key == KEY_MOVE_BACKWORD || key == KEY_MOVE_FORWORD)
			{
				m_keyPressed = true;
				m_keyType |= constants->KEY[key];
			}
			else if (key == KEY_LSHIFT)
			{
				m_keyType |= LSHIFT;
			}
		}
		else
		{
			if (key == KEY_LSHIFT)
			{
				m_keyType &= ~LSHIFT;
			}
			else
			{
				m_keyType &= ~constants->KEY[key];
				if (m_keyType == 0)
				{
					if (m_player->IsMove())
						m_player->Idle();
					m_keyPressed = false;
				}
			}
		}
	}
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	if (!m_isPause && !m_player->IsDie())
	{
		for (auto it : m_listButton)
		{
			if (it->IsEnable() && !m_isUltimate)
			{
				(it)->HandleTouchEvents(x, y, bIsPressed);
				if ((it)->IsHandle()) break;
			}
		}
		// prepare ultimate if click button ultimate
		if (m_listButton[4]->IsClicked(x, y, bIsPressed) && m_player->HasUltimate())
		{
			m_isUltimate = true;
			m_listSprite2D[3]->SetEnable(true);
		}
		// using ultimate
		if (!bIsPressed && m_isUltimate)
		{
			m_isUltimate = false;
			m_listSprite2D[3]->SetEnable(false);
			// anim ultimate
			m_listSpriteAnim[2]->ResetCurrentTime();
			m_listSpriteAnim[2]->SetCurrentFrame(0);
			m_listSpriteAnim[2]->Set2DPosition(x, y);
			m_listSpriteAnim[2]->SetEnable(true);

			score += m_player->Ultimate(m_listEnermy, { 1.0f * x, 1.0f * y });
			if (isEfMusicOn)
				ultimateSound = ResourceManagers::GetInstance()->PlaySound("ultimate");
		}
		return;
	}
	if (!m_isEnd)
	{
		for(int i = 0; i < 3; ++i)
		{
			if (m_listButton[i]->IsEnable())
			{
				m_listButton[i]->HandleTouchEvents(x, y, bIsPressed);
				if (m_listButton[i]->IsHandle()) break;
			}
		}
		return;
	}
	// if end game, only handle button quit
	m_listButton[2]->HandleTouchEvents(x, y, bIsPressed);
	m_listButton[5]->HandleTouchEvents(x, y, bIsPressed);
}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
	// button radius ultimate
	if (m_isUltimate)
		m_listSprite2D[3]->Set2DPosition(x, y);
}

void GSPlay::Update(float deltaTime)
{
	if (!m_isPause)
	{
		// update hero and enermy
		m_player->Update(deltaTime);
		for (auto e : m_listEnermy)
		{
			e->Update(m_player, deltaTime);
		}
		// if hero isnt die, he can move and find enermy
		if (!m_player->IsDie())
		{
			// processing hero move
			if (!m_player->IsAttack())
			{
				if (m_keyPressed)
				{
					if (m_keyType & LSHIFT && !m_player->IsTired())
						m_player->Run();
					else
						m_player->Walk();
					MoveCharacter(deltaTime * m_player->GetSpeed());
				}
			}
			// find nearest enermy
			enermy = FindNearestEnermy();
			if (enermy)
			{
				m_listSprite2D[1]->SetEnable(true);
				m_listSprite2D[1]->Set2DPosition(enermy->Get2DPosition());
			}
			else
			{
				m_listSprite2D[1]->SetEnable(false);
			}
			// update spr2d
			UpdateSprite2D();
			// update anim
			UpdateAnim(deltaTime);
			// update text
			UpdateText();
			// generate random enermy
			RandomEnermy(deltaTime);
		}
		else // enermy win
		{
			EnermyWin();
		}
		return;
	}
	if (m_player->IsDie())
	{
		for (auto e : m_listEnermy)
		{
			e->Update(m_player, deltaTime);
		}
	}
	
}

void GSPlay::Draw()
{
	m_background->Draw();
	// draw enermy higher than hero
	for (auto e : m_listEnermy)
	{
		if (e->Get2DPosition().y <= m_player->Get2DPosition().y)
			e->Draw();
	}
	m_player->Draw();
	// draw enermy shorter than hero
	for (auto e : m_listEnermy)
	{
		if (e->Get2DPosition().y > m_player->Get2DPosition().y)
			e->Draw();
	}
	// draw health bar anim
	for (auto anim : m_listHealthBarAnim)
	{
		if (anim->IsEnable())
			anim->Draw();
	}
	// draw sprite 2d
	for (auto spr2d : m_listSprite2D)
	{
		if (spr2d->IsEnable())
			spr2d->Draw();
	}
	for (auto miniEnermy : m_listMiniEnermy)
	{
		if (miniEnermy->IsEnable())
			miniEnermy->Draw();
	}
	m_miniHero->Draw();
	// draw button
	for (auto b : m_listButton)
	{
		if (b->IsEnable())
			b->Draw();
	}
	// draw sprite anim
	for (auto anim : m_listSpriteAnim)
	{
		if (anim->IsEnable())
			anim->Draw();
	}
	if (isTextFloatOn)
	{
		// draw text health
		for (auto text : m_listText)
		{
			if (text->IsEnable())
				text->Draw();
		}
	}
	// draw if endgame
	if (m_player->IsDie())
	{
		m_grayBG->Draw();
		m_listButton[2]->Draw();
		m_listButton[5]->Draw();
		m_stunHero->Draw();
	}
	// draw text score
	m_scoreText->Draw();
}

void GSPlay::UpdateSprite2D()
{
	Vector2 margin = { 50, screenHeight * 1.0f - 50 };
	// update mini hero
	Vector2 p = ConvertPosEnermy(m_player->Get2DPosition(), margin);
	m_miniHero->Set2DPosition(p);
	// update mini enermy
	for (int i = 0; i < m_listEnermy.size(); ++i)
	{
		if (!m_listEnermy[i]->IsDie())
		{
			Vector2 p = ConvertPosEnermy(m_listEnermy[i]->Get2DPosition(), margin);
			m_listMiniEnermy[i]->Set2DPosition(p);
			m_listMiniEnermy[i]->SetEnable(true);
		}
		else
			m_listMiniEnermy[i]->SetEnable(false);
	}
}

void GSPlay::UpdateAnim(float deltaTime)
{
	// update kill cd
	m_listSpriteAnim[0]->SetCurrentFrame(10 - (int)(m_player->GetTimeDelayAttack() * 10));
	m_listSpriteAnim[1]->SetCurrentFrame(10 - (int)(m_player->GetTimeDelayUltimate() / constants->hTIME_ULTIMATE * 10));
	// update ultimate effect
	if (m_listSpriteAnim[2]->IsLastFrame())
		m_listSpriteAnim[2]->SetEnable(false);
	else
		m_listSpriteAnim[2]->Update(deltaTime);
	// status bar
	m_listSpriteAnim[3]->SetEnable(!m_player->NeedTimeTired());
	m_listSpriteAnim[3]->SetCurrentFrame(max(0, (int)m_player->GetTimeTired() - 1));
	// health bar hero
	m_listSpriteAnim[4]->SetCurrentFrame(10 * m_player->GetHeal() / m_player->GetHealMax());
	// health bar enermy
	for (int i = 0; i < m_listHealthBarAnim.size(); ++i)
	{
		if (m_listEnermy[i]->IsDie())
		{
			m_listHealthBarAnim[i]->SetEnable(false);
		}
		else
		{
			int percentHeal = 10 * m_listEnermy[i]->GetHeal() / m_listEnermy[i]->GetHealMax();
			m_listHealthBarAnim[i]->SetCurrentFrame(percentHeal);
			// update position health bar
			Vector2 ePos = m_listEnermy[i]->Get2DPosition();
			m_listHealthBarAnim[i]->Set2DPosition(ePos - Vector2(0, 75));
			m_listHealthBarAnim[i]->SetEnable(true);
		}
	}
}

void GSPlay::UpdateText()
{
	// update text score
	m_scoreText->setText("Score: " + std::to_string(score));
	if (isTextFloatOn)
	{
		// update text health hero
		m_listText[constants->NUM_ENERMY]->setText(std::to_string(m_player->GetHeal()) + "/" + std::to_string(m_player->GetHealMax()));
		m_listText[constants->NUM_ENERMY]->Set2DPosition(m_player->Get2DPosition() - Vector2(25, 70));
		// update text health enermy
		for (int i = 0; i < m_listEnermy.size(); ++i)
		{
			if (!m_listEnermy[i]->IsDie())
			{
				m_listText[i]->setText(m_listEnermy[i]->GetStringHeal());
				m_listText[i]->Set2DPosition(m_listEnermy[i]->Get2DPosition() - Vector2(25, 70));
				m_listText[i]->SetEnable(true);
			}
			else
			{
				m_listText[i]->SetEnable(false);
			}
		}
	}
}

Vector2 GSPlay::ConvertPosEnermy(Vector2 pos, Vector2 margin)
{
	Vector2 mg = margin + Vector2(m_miniMapSize.x, -m_miniMapSize.y) * 0.5;
	Vector2 p = (pos - m_background->Get2DPosition()) / constants->mSCALE;
	return mg + Vector2(p.x, p.y);
}

Vector2 GSPlay::ConvertPosHero(Vector2 pos, Vector2 margin)
{
	Vector2 mg = margin + Vector2(m_miniMapSize.x, -m_miniMapSize.y) * 0.5;
	Vector2 p = (pos - m_background->Get2DPosition()) / constants->mSCALE;
	return mg + Vector2(p.x, p.y);
}

void GSPlay::RandomEnermy(float deltaTime)
{
	m_time += deltaTime;
	if (m_time >= constants->pTIME_RANDOM_ENERMY)
	{
		for (auto e : m_listEnermy)
		{
			if (e->IsDie())
			{
				e->Reset(score);
			}
		}
		m_time -= constants->pTIME_RANDOM_ENERMY;
	}
}

void GSPlay::EnermyWin()
{
	// hide health bar
	m_listSpriteAnim[4]->SetEnable(false);
	// hide text heal
	if (isTextFloatOn)
		m_listText[constants->NUM_ENERMY]->SetEnable(false);
	// display end game
	// button exit
	m_listButton[2]->Set2DPosition(screenWidth / 2 + 120, screenHeight - 200);
	m_listButton[2]->SetSize(120, 120);
	// button replay
	m_listButton[5]->Set2DPosition(screenWidth / 2 - 120, screenHeight - 200);
	// text score
	auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	auto font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_scoreText = std::make_shared<Text>(shader, font, "Score: " + std::to_string(score), TEXT_COLOR::PURPLE, 3);
	m_scoreText->Set2DPosition(screenWidth / 2 - 175, 175);

	for (auto e : m_listEnermy)
	{
		if (!e->IsDie())
			e->Win();
	}
	if (m_player->IsLastFrame())
	{
		m_isPause = m_isEnd = true;
		ResourceManagers::GetInstance()->StopAllSound();
		if (isBgMusicOn)
			endSound = ResourceManagers::GetInstance()->PlaySound("endgame", true);
	}
}

void GSPlay::MoveEnermy(Vector2 dVector)
{
	for (auto e : m_listEnermy)
	{
		Vector2 oldPos = e->Get2DPosition();
		e->Set2DPosition(oldPos + dVector);
	}
}

Vector2 GSPlay::MoveMap(float d)
{
	// pos background
	Vector2 oldPos = m_background->Get2DPosition();
	Vector2 newPos = oldPos;
	if (m_keyType & LEFT)
	{
		m_player->SetDirection(D_LEFT);
		newPos.x = oldPos.x + d;
		if (m_mapSize.x * 0.5 - newPos.x < 0)
			newPos.x = m_mapSize.x * 0.5;
	}
	else if (m_keyType & RIGHT)
	{
		m_player->SetDirection(D_RIGHT);
		newPos.x = oldPos.x - d;
		if (m_mapSize.x * 0.5 + newPos.x < screenWidth)
			newPos.x = screenWidth - m_mapSize.x * 0.5;
	}
	if (m_keyType & DOWN)
	{
		newPos.y = oldPos.y - d;
		if (m_mapSize.y * 0.5 + newPos.y < screenHeight)
			newPos.y = screenHeight - m_mapSize.y * 0.5;
	}
	else if (m_keyType & UP)
	{
		newPos.y = oldPos.y + d;
		if (m_mapSize.y * 0.5 - newPos.y < 0)
			newPos.y = m_mapSize.y * 0.5;
	}
	Vector2 dVector = newPos - oldPos;
	if (dVector.x != 0 && dVector.y != 0)
	{
		dVector *= 0.7071f;
		m_background->Set2DPosition(oldPos + dVector);
	}
	else
		m_background->Set2DPosition(newPos);
	return dVector;
}

void GSPlay::MoveCharacter(float d)
{
	Vector2 dVector = MoveMap(d);
	MoveEnermy(dVector);
}

void GSPlay::InitSprite2Ds()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	// gray color
	auto texture = ResourceManagers::GetInstance()->GetTexture("gray");
	m_grayBG = std::make_shared<Sprite2D>(model, shader, texture);
	m_grayBG->Set2DPosition(screenWidth / 2, screenHeight / 2);
	m_grayBG->SetSize(screenWidth, screenHeight);

	// stun hero
	std::string hNames[3] = { "archer", "icewizard", "knight" };
	texture = ResourceManagers::GetInstance()->GetTexture(hNames[idHero] + "_stun");
	m_stunHero = std::make_shared<Sprite2D>(model, shader, texture);
	m_stunHero->Set2DPosition(screenWidth / 2, screenHeight - 400);
	m_stunHero->SetSize(320, 320);

	// mini background [0]
	Vector2 margin = { 50, screenHeight * 1.0f - 50 };
	texture = ResourceManagers::GetInstance()->GetTexture("bg_game");
	auto spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->Set2DPosition(margin.x + m_miniMapSize.x * 0.5, margin.y - m_miniMapSize.y * 0.5);
	spr2d->SetSize(m_miniMapSize);
	m_listSprite2D.push_back(spr2d);

	// target scope [1]
	texture = ResourceManagers::GetInstance()->GetTexture("target_scope");
	spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->SetSize(50, 50);
	spr2d->SetEnable(false);
	m_listSprite2D.push_back(spr2d);

	// gray color [2]
	texture = ResourceManagers::GetInstance()->GetTexture("gray");
	spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->Set2DPosition(screenWidth / 2, screenHeight / 2);
	spr2d->SetSize(screenWidth, screenHeight);
	spr2d->SetEnable(false);
	m_listSprite2D.push_back(spr2d);

	// radius ultimate [3]
	texture = ResourceManagers::GetInstance()->GetTexture("radius_ultimate");
	spr2d = std::make_shared<Sprite2D>(model, shader, texture);
	spr2d->SetSize(2 * constants->hRADIUS_ULTIMATE, 2 * constants->hRADIUS_ULTIMATE);
	spr2d->SetEnable(false);
	m_listSprite2D.push_back(spr2d);

	// list mini enermy
	texture = ResourceManagers::GetInstance()->GetTexture("red_dot");
	for (auto e : m_listEnermy)
	{
		spr2d = std::make_shared<Sprite2D>(model, shader, texture);
		spr2d->Set2DPosition(ConvertPosEnermy(e->Get2DPosition(), margin));
		spr2d->SetSize(5, 5);
		m_listMiniEnermy.push_back(spr2d);
	}
	// mini hero
	texture = ResourceManagers::GetInstance()->GetTexture("green_dot");
	m_miniHero = std::make_shared<Sprite2D>(model, shader, texture);
	m_miniHero->Set2DPosition(ConvertPosEnermy(m_player->Get2DPosition(), margin));
	m_miniHero->SetSize(5, 5);
}

void GSPlay::InitGameButtons()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	// button pause [0]
	auto texture = ResourceManagers::GetInstance()->GetTexture("button_pause");
	auto button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->CurrentState()->Pause();
	});
	m_listButton.push_back(button);

	// button play [1]
	texture = ResourceManagers::GetInstance()->GetTexture("button_resume");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->CurrentState()->Resume();
	});
	button->SetEnable(false);
	m_listButton.push_back(button);

	// button quit [2]
	texture = ResourceManagers::GetInstance()->GetTexture("button_quit_2");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 60, 60);
	button->SetSize(60, 60);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
	});
	m_listButton.push_back(button);

	// button attack skill [3]
	std::string hNames[3] = { "archer", "icewizard", "knight" };
	texture = ResourceManagers::GetInstance()->GetTexture("button_attack_" + hNames[idHero]);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 130, screenHeight - 50);
	button->SetSize(75, 75);
	button->SetOnClick([]() {
		if (m_player->Attackable())
		{
			if (isEfMusicOn)
				attackSound = ResourceManagers::GetInstance()->PlaySound("attack");
			score += m_player->Attack(enermy);
		}
	});
	m_listButton.push_back(button);

	//button ultimate skill [4]
	texture = ResourceManagers::GetInstance()->GetTexture("button_ultimate_" + hNames[idHero]);
	button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(screenWidth - 55, screenHeight - 125);
	button->SetSize(75, 75);
	button->SetOnClick([]() {
	});
	m_listButton.push_back(button);

	//button replay [5]
	texture = ResourceManagers::GetInstance()->GetTexture("button_replay");
	button = std::make_shared<GameButton>(model, shader, texture);
	button->SetSize(120, 120);
	button->SetOnClick([]() {
		GameStateMachine::GetInstance()->PopState();
		GameStateMachine::GetInstance()->PushState(StateTypes::STATE_Play);
	});
	button->SetEnable(false);
	m_listButton.push_back(button);
}

void GSPlay::InitSpriteAnim()
{
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");

	// kill cd attack [0]
	auto texture = ResourceManagers::GetInstance()->GetTexture("kill_cd");
	auto anim = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0f / 11);
	anim->Set2DPosition(screenWidth - 130, screenHeight - 50);
	anim->SetSize(75, 75);
	m_listSpriteAnim.push_back(anim);

	// kill cd ultimate [1]
	texture = ResourceManagers::GetInstance()->GetTexture("kill_cd");
	anim = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0f / 11);
	anim->Set2DPosition(screenWidth - 55, screenHeight - 125);
	anim->SetSize(75, 75);
	m_listSpriteAnim.push_back(anim);

	// ultimate [2]
	std::string hNames[3] = { "archer", "icewizard", "knight" };
	texture = ResourceManagers::GetInstance()->GetTexture("ultimate_" + hNames[idHero]);
	anim = std::make_shared<SpriteAnimation>(model, shader, texture, 4, 0.25f);
	anim->SetSize(300, 300);
	anim->SetEnable(false);
	m_listSpriteAnim.push_back(anim);

	// status bar [3]
	texture = ResourceManagers::GetInstance()->GetTexture("status_bar");
	anim = std::make_shared<SpriteAnimation>(model, shader, texture, 5, 0.2f);
	anim->Set2DPosition(102.5, 125);
	anim->SetSize(145, 10);
	m_listSpriteAnim.push_back(anim);

	// health bar for hero [4]
	texture = ResourceManagers::GetInstance()->GetTexture("health_bar");
	anim = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0f / 11);
	anim->Set2DPosition(m_player->Get2DPosition() - Vector2(0, 75));
	anim->SetSize(100, 13);
	m_listSpriteAnim.push_back(anim);

	// health bar for enermy
	for (auto e : m_listEnermy)
	{
		texture = ResourceManagers::GetInstance()->GetTexture("health_bar");
		anim = std::make_shared<SpriteAnimation>(model, shader, texture, 11, 1.0f / 11);
		anim->Set2DPosition(e->Get2DPosition() - Vector2(0, 75));
		anim->SetSize(100, 13);
		m_listHealthBarAnim.push_back(anim);
	}
}

void GSPlay::InitText()
{
	auto shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	// text score game
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
	m_scoreText = std::make_shared< Text>(shader, font, "Score: 0", TEXT_COLOR::RED, 1.5);
	m_scoreText->Set2DPosition(screenWidth / 2 - 75, 60);

	if (isTextFloatOn)
	{
		for (auto e : m_listEnermy)
		{
			std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("arialbd");
			auto text = std::make_shared< Text>(shader, font, e->GetStringHeal(), TEXT_COLOR::PURPLE, 0.55);
			text->Set2DPosition(e->Get2DPosition() - Vector2(25, 70));
			m_listText.push_back(text);
		}
		// health text hero [NUM_ENERMY]
		font = ResourceManagers::GetInstance()->GetFont("arialbd");
		auto text = std::make_shared< Text>(shader, font, std::to_string(m_player->GetHeal()) + "/" + std::to_string(m_player->GetHealMax()), TEXT_COLOR::PURPLE, 0.55);
		text->Set2DPosition(m_player->Get2DPosition() - Vector2(25, 70));
		m_listText.push_back(text);
	}
}

std::shared_ptr<Enermy> GSPlay::FindNearestEnermy()
{
	enermy = nullptr;
	int dAttack = m_player->GetDistanceAttack();
	Vector2 heroPos = m_player->Get2DPosition();
	for (auto e : m_listEnermy)
	{
		if (!e->IsDie()) {
			Vector2 enermyPos = e->Get2DPosition();
			int d = (heroPos - enermyPos).Length();
			if (d < dAttack)
			{
				dAttack = d;
				enermy = e;
			}
		}
	}
	return enermy;
}
