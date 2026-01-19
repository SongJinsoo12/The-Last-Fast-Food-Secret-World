#include "InputGame.h"
#include <iostream>

GameInput_M::Input g_Input;

namespace GameInput_M {
	int Input::isClick()
	{
		std::cout << "호출" << std::endl;
		if (m_ISMouseClick[(int)GameInput_M::MouseValue::Right])
		{
			std::cout << "우클릭 값" << (int)GameInput_M::MouseValue::Left << std::endl;
			return (int)GameInput_M::MouseValue::Right;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Left])
		{
			std::cout << "좌클릭 값" << (int)GameInput_M::MouseValue::Left << std::endl;
			return (int)GameInput_M::MouseValue::Left;
		}
		else if (m_ISMouseClick[(int)GameInput_M::MouseValue::Heel])
		{
			std::cout << "휠 값" << (int)GameInput_M::MouseValue::Left << std::endl;
			return (int)GameInput_M::MouseValue::Heel;
		}
		else
			return 0;
	}
	int Input::isHeel()
	{
		return m_MouseHeelValue;
	}
	void Input::UpdateProcess(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
    {

		switch (iMessage) {

		case WM_LBUTTONDOWN:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = true;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			std::cout << "마우스 좌클릭 누르기" << std::endl;
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case WM_LBUTTONUP:
			m_ISMouseClick[(int)GameInput_M::MouseValue::Left] = false;
			m_MousePosX = LOWORD(lParam);
			m_MousePosY = HIWORD(lParam);
			std::cout << "마우스 좌클릭 끝내기" << std::endl;
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
			switch (wParam) {
			case VK_RIGHT:

			}
		}

    }
}