#pragma once
#include "GameManager/Singleton.h"
#include "GameConfig.h"
#include "gamestatebase.h"
#include <vector>

class SpriteAnimation;

enum CharacterStateTypes
{
	CHARACTER_STATE_Idle = 0,
	CHARACTER_STATE_Walk,
	CHARACTER_STATE_Run,
	CHARACTER_STATE_Attack,
	CHARACTER_STATE_Win,
	CHARACTER_STATE_Die,
	CHARACTER_STATE_Jump,
	CHARACTER_STATE_Hurt
};

class CharacterState : public GameStateBase, public CSingleton<CharacterState>
{

public:
	CharacterState(int idCharacter);
	~CharacterState();


public:

	void ChangeState(CharacterStateTypes cst);

	inline std::shared_ptr<SpriteAnimation>	CurrentState()const
	{
		return m_pActiveState;
	}

	void Init();
	void Exit();

	void Pause();
	void Resume();

	void HandleEvents();
	void HandleKeyEvents(int key, bool bIsPressed);
	void HandleTouchEvents(int x, int y, bool bIsPressed);
	void Update(float deltaTime);
	void Draw();

private:

	std::vector<std::shared_ptr<SpriteAnimation>> m_listState;
	std::shared_ptr<SpriteAnimation> m_pActiveState;
	char **arrayCharacterName;
};