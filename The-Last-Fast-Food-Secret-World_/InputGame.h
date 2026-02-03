#pragma once
#include <windows.h>
#include "Singleton.h"

#define INPUT GameInput_M::Input::GetInstance()

namespace GameInput_M {
	enum class MouseValue {
		Left = 1,
		Heel,
		Right
	};

	class Input : public Singleton<Input>
	{
		friend class Singleton<Input>;
	public:
		virtual int isClick();
		virtual int isHeel();
		virtual bool isKeyboard(int p_key);
		virtual void GetMousePos(int* p_x, int* p_y);

		virtual void UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


	protected:
		bool m_ISMouseClick[(int)GameInput_M::MouseValue::Right + 1];
		int m_MousePosX;
		int m_MousePosY;
		int m_MouseHeelDelta;

		bool m_KeyBoardValue[256];
	};

}