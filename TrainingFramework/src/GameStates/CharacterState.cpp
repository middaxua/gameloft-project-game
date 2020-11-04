#include "CharacterState.h"
#include "Sprite2D.h"
#include "SpriteAnimation.h"

CharacterState::CharacterState(int idCharacter)
{
	arrayCharacterName = new char*[3];
	arrayCharacterName[0] = new char[15] { "archer_0" };
	arrayCharacterName[1] = new char[15] { "icewizard_0" };
	arrayCharacterName[2] = new char[15] { "knight_0"};

	std::shared_ptr<SpriteAnimation> c = nullptr;
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D");
	auto shader = ResourceManagers::GetInstance()->GetShader("Animation");
	std::shared_ptr<Texture> texture = nullptr;
	std::shared_ptr<SpriteAnimation> tmpCharacter = nullptr;
	int len = strlen(arrayCharacterName[idCharacter]);
	for (int i = 0; i < 8; ++i) {
		arrayCharacterName[idCharacter][len - 1] = i + '0';
		texture = ResourceManagers::GetInstance()->GetTexture(arrayCharacterName[idCharacter]);
		tmpCharacter = std::make_shared<SpriteAnimation>(model, shader, texture, 10, 0.1f);
		tmpCharacter->Set2DPosition(240, 400);
		tmpCharacter->SetSize(128, 128);
		m_listState.push_back(tmpCharacter);
	}
	m_pActiveState = m_listState[0];
}


CharacterState::~CharacterState()
{
	for (int i = 0; i < 3; ++i)
	{
		delete[] arrayCharacterName[i];
	}
	delete[] arrayCharacterName;
}

void CharacterState::ChangeState(CharacterStateTypes cst)
{
	switch (cst)
	{
	case CHARACTER_STATE_Idle:
		m_pActiveState = m_listState[0];
		break;
	case CHARACTER_STATE_Walk:
		m_pActiveState = m_listState[1];
		break;
	case CHARACTER_STATE_Run:
		m_pActiveState = m_listState[2];
		break;
	case CHARACTER_STATE_Attack:
		m_pActiveState = m_listState[3];
		break;
	case CHARACTER_STATE_Win:
		m_pActiveState = m_listState[4];
		break;
	case CHARACTER_STATE_Die:
		m_pActiveState = m_listState[5];
		break;
	case CHARACTER_STATE_Jump:
		m_pActiveState = m_listState[6];
		break;
	case CHARACTER_STATE_Hurt:
		m_pActiveState = m_listState[7];
		break;
	default:
		break;
	}
}

void CharacterState::Init()
{
}

void CharacterState::Exit()
{
}


void CharacterState::Pause()
{

}

void CharacterState::Resume()
{

}


void CharacterState::HandleEvents()
{

}

void CharacterState::HandleKeyEvents(int key, bool bIsPressed)
{

}

void CharacterState::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	
}

void CharacterState::Update(float deltaTime)
{
	m_pActiveState->Update(deltaTime);
}

void CharacterState::Draw()
{
	m_pActiveState->Draw();
}