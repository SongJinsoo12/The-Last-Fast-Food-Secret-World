#pragma once
#include <windows.h>
#include "Singleton.h"

<<<<<<< HEAD
#define m_Input GameInput_M::Input::GetInstance()

namespace GameInput_M {
	enum class MouseValue {
		Left=1,
=======
namespace GameInput_M {
	enum class MouseValue {
		Left = 1,
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
		Heel,
		Right
	};

	class Input : public Singleton<Input>
	{
		friend class Singleton<Input>;
<<<<<<< HEAD
	public:
		virtual int isClick();
		virtual int isHeel();
		//virtual bool isKeyboard(char p_key);

		virtual int getMouseX() 
		{
			return m_MousePosX;
		}
		virtual int getMouseY()
		{
			return m_MousePosY;
		}

		virtual void UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

=======

	public:
		Input();

		virtual int isClick();
		virtual int isHeel();
		virtual bool isKeyboard(int p_key);

		virtual void UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
		virtual void GetMousePos(int* p_x, int* p_y);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f

	protected:
		bool m_ISMouseClick[(int)GameInput_M::MouseValue::Right + 1];
		int m_MousePosX;
		int m_MousePosY;
<<<<<<< HEAD
		int m_MouseHeelDelta;

		bool m_KeyArr[256];
	};

}
=======
		int m_MouseHeelValue;

		bool m_keyBoardValue[256];
	};

}
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
