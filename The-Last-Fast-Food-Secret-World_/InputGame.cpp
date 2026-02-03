#include "InputGame.h"
#include <iostream>

namespace GameInput_M {
	Input::Input()
	{
		m_MousePosX = 0; m_MousePosY = 0;
		m_MouseHeelValue = 0;
	}
	int Input::isClick()
	{
		if (m_ISMouseClick[(int)GameInput_M::MouseValue::Right])
		{
			return (int)GameInput_M::MouseValue::Right;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Left])
		{
			return (int)GameInput_M::MouseValue::Left;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Heel])
		{
			return (int)GameInput_M::MouseValue::Heel;
		}
		else
			return 0;
	}
	int Input::isHeel()
	{
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

		switch (iMessage) {

		case WM_LBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_LBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_RBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_RBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_MOUSEWHEEL:
			m_MouseHeelValue = GET_WHEEL_DELTA_WPARAM(wParam);
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_KEYDOWN:
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
