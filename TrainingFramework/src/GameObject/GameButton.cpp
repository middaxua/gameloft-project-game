#include "GameButton.h"


GameButton::GameButton(std::shared_ptr<Models> model, std::shared_ptr<Shaders> shader, std::shared_ptr<Texture> texture)
	: Sprite2D(model, shader, texture)
{
	m_pBtClick = nullptr;
	m_isHandle = false;
	m_isEnable = true;
}

GameButton::~GameButton()
{
}

void GameButton::SetOnClick(void(*pBtClickFun)())
{
	m_pBtClick = pBtClickFun;
}

void GameButton::HandleTouchEvents(GLint x, GLint y, bool bIsPressed)
{
	if (!bIsPressed) {
		m_isHandle = false;
		if ((x > m_Vec2DPos.x - m_iWidth / 2) && (x < m_Vec2DPos.x + m_iWidth / 2) && (y > m_Vec2DPos.y - m_iHeight / 2) && (y < m_Vec2DPos.y + m_iHeight / 2))
		{
			m_pBtClick();
			m_isHandle = true;
		}
	}
}

bool GameButton::IsHandle()
{
	return m_isHandle;
}

bool GameButton::IsClicked(GLint x, GLint y, bool bIsPressed)
{
	if (bIsPressed)
	{
		return (x > m_Vec2DPos.x - m_iWidth / 2) && (x < m_Vec2DPos.x + m_iWidth / 2) && (y > m_Vec2DPos.y - m_iHeight / 2) && (y < m_Vec2DPos.y + m_iHeight / 2);
	}
}

//bool GameButton::IsEnable()
//{
//	return m_isEnable;
//}
//
//void GameButton::SetEnable(bool e)
//{
//	m_isEnable = e;
//}
