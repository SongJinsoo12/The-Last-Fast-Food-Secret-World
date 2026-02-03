#include "InputGame.h"
#include <iostream>

namespace GameInput_M {
<<<<<<< HEAD
	int Input::isClick()
	{
		//std::cout << "호출" << std::endl;
		if (m_ISMouseClick[(int)GameInput_M::MouseValue::Right])
		{
			std::cout << "우클릭 값" << (int)GameInput_M::MouseValue::Left << std::endl;
=======
	Input::Input()
	{
		m_MousePosX = 0; m_MousePosY = 0;
		m_MouseHeelValue = 0;
	}
	int Input::isClick()
	{
		if (m_ISMouseClick[(int)GameInput_M::MouseValue::Right])
		{
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			return (int)GameInput_M::MouseValue::Right;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Left])
		{
<<<<<<< HEAD
			std::cout << "좌클릭 값" << (int)GameInput_M::MouseValue::Left << std::endl;
=======
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			return (int)GameInput_M::MouseValue::Left;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Heel])
		{
<<<<<<< HEAD
			std::cout << "휠 값" << (int)GameInput_M::MouseValue::Left << std::endl;
=======
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			return (int)GameInput_M::MouseValue::Heel;
		}
		else
			return 0;
	}
	int Input::isHeel()
	{
<<<<<<< HEAD
		return m_MouseHeelDelta;
	}
	void Input::UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
    {
=======
		return m_MouseHeelValue;
	}
	bool Input::isKeyboard(int p_key)
	{
		if(0 > p_key || p_key >= 256)
			return false;
		return m_keyBoardValue[p_key];
	}
	void Input::UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f

		switch (iMessage) {

		case WM_LBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
			std::cout << "마우스 좌클릭 누르기" << std::endl;
=======
			InvalidateRect(hWnd, NULL, FALSE);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			break;

		case WM_LBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
			std::cout << "마우스 좌클릭 끝내기" << std::endl;
=======
			InvalidateRect(hWnd, NULL, FALSE);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			break;

		case WM_RBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
=======
			InvalidateRect(hWnd, NULL, FALSE);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			break;

		case WM_RBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
=======
			InvalidateRect(hWnd, NULL, FALSE);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			break;

		case WM_MBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
=======
			InvalidateRect(hWnd, NULL, FALSE);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			break;

		case WM_MBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
<<<<<<< HEAD
			break;

		case WM_MOUSEWHEEL:
			m_MouseHeelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
=======
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MOUSEWHEEL:
			m_MouseHeelValue = GET_WHEEL_DELTA_WPARAM(wParam);
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_KEYDOWN:
<<<<<<< HEAD
			m_KeyArr[wParam] = true;
			break;

		case WM_KEYUP:
			if (m_KeyArr[wParam])
				m_KeyArr[wParam] = false;
			break;
		}

    }
}
=======
			m_keyBoardValue[wParam] = true;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_KEYUP:
			m_keyBoardValue[wParam] = false;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}

	}
	void Input::GetMousePos(int* p_x, int* p_y)
	{
		*p_x = this->m_MousePosX;
		*p_y = this->m_MousePosY;
	}
}
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
