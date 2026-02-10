#include "InputGame.h"
#include <iostream>

namespace GameInput_M {
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

	bool Input::isOneClick(MouseValue p_value)
	{
		if (m_ISMouseClick[(int)p_value])
		{
			m_ISMouseClick[(int)p_value] = false;
			return true;
		}
		else
			return false;
	}

	int Input::isHeel()
	{
		return m_MouseHeelDelta;
	}

	bool Input::isKeyboard(int p_key)
	{
		if (0 > p_key || p_key >= 256)
			return false;

		if (m_KeyBoardValue[p_key])
		{
			m_KeyBoardValue[p_key] = false;
			return true;
		}
		else
			return false;
	}

	void Input::GetMousePos(int* p_x, int* p_y)
	{
		*p_x = this->m_MousePosX;
		*p_y = this->m_MousePosY;
	}

	void Input::UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
	{

		switch (iMessage) {

		case WM_LBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			std::cout << "마우스 좌클릭 누르기" << std::endl;
			break;

		case WM_LBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			std::cout << "마우스 좌클릭 끝내기" << std::endl;
			break;

		case WM_RBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			break;

		case WM_RBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Right] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			break;

		case WM_MBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			break;

		case WM_MBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Heel] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			break;

		case WM_MOUSEWHEEL:
			m_MouseHeelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			break;

		case WM_KEYDOWN:
			m_KeyBoardValue[wParam] = true;
			break;

		case WM_KEYUP:
			m_KeyBoardValue[wParam] = false;
			break;
		}

	}
}