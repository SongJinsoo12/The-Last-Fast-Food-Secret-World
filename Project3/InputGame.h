#pragma once
#include <windows.h>

namespace GameInput_M {
	enum class MouseValue {
		Left=1,
		Heel,
		Right
	};

	class Input
	{
	public:
		virtual int isClick();
		virtual int isHeel();
		//virtual bool isKeyboard(char p_key);

		virtual void UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


	protected:
		bool m_ISMouseClick[(int)GameInput_M::MouseValue::Right + 1];
		int m_MousePosX;
		int m_MousePosY;
		int m_MouseHeelValue;
	};

}

extern GameInput_M::Input g_Input;