#pragma once
#include <windows.h>
#include "Singleton.h"

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
		Input();

		virtual int isClick();
		virtual int isHeel();
		virtual bool isKeyboard(int p_key);

		virtual void UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		virtual void GetMousePos(int* p_x, int* p_y);

	protected:
		bool m_ISMouseClick[(int)GameInput_M::MouseValue::Right + 1];
		int m_MousePosX;
		int m_MousePosY;
		int m_MouseHeelValue;

		bool m_keyBoardValue[256];
	};

}
