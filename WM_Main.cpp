#include <windows.h>										// 헤더
#include <iostream>
#include "DeckBuilding.h"
#include "CardGacha.h"
#include "MainGame.h"
#include "Shop.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;											// 인스턴스 핸들
LPCTSTR lpszClass = TEXT("Secret World");					// 제목 표시줄에 표시

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
#ifdef _DEBUG
	if (AllocConsole()) {
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		freopen_s(&fp, "CONOUT$", "w", stderr); // 에러 출력도 콘솔로
		std::ios::sync_with_stdio(); // cout과 printf를 섞어 쓸 때 유용
	}
#endif

	HWND hWnd;												// 윈도우 핸들 선언
	MSG Message;											// 메시지 구조체 변수 선언
	WNDCLASS WndClass;										// Windows Class 구조체 변수 선언
	g_hInst = hInstance;									// hInstance값을 외부에서도 사용되게 전역 변수에 저장

	WndClass.cbClsExtra = 0;								// 예약 영역, 지금 사용 x
	WndClass.cbWndExtra = 0;								// 예약 영역
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	// 배경 색 지정
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// 마우스 포인터 모양 지정
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// 커서 아이콘 지정
	WndClass.hInstance = hInstance;							// 클래스 등록하는 프로그램 번호
	WndClass.lpfnWndProc = WndProc;							// 메시지 처리 함수 지정
	WndClass.lpszClassName = lpszClass;						// 클래스 이름 지정
	WndClass.lpszMenuName = NULL;							// 메뉴 지정
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;		// 스타일 정의

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, // 윈도우 생성
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT
		/*100,100,500,500*/, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) { //Queue에 있는 메시지 읽어들임
		TranslateMessage(&Message); // 키보드 입력 메시지 가공
		DispatchMessage(&Message); // 메시지 처리
	}
	return (int)Message.wParam; // 탈출 코드, 프로그램 종료
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;////////////////////////
	PAINTSTRUCT ps;
	static DeckBuilding deck;
	static CardGacha gacha;
	static WCHAR print[256];
	static MainGame mg;
	static RECT rt;
	static RECT a;
	static HPEN hPen, oldPen;		//펜정보
	static int screen = -1;			//현재화면번호
	static Shop shop;
	static HWND b;
	static BOOL isGrad = FALSE;		//격자눈금 온오프용

	switch (iMessage)
	{
	case WM_CREATE:
	{
		srand(time(NULL));
		GetClientRect(hWnd, &rt);
		a.left = 20, a.right = 120, a.top = 20, a.bottom = 45;
		//푸시버튼
		b = CreateWindow(TEXT("button"), TEXT("덱편집"), WS_CHILD | WS_VISIBLE		//(TEXT("button"), TEXT(버튼내용), WS_CHILD | WS_VISIBLE
			| BS_PUSHBUTTON, 20, 20, 100, 25, hWnd, (HMENU)0, g_hInst, NULL);	//| BS_PUSHBUTTON, startx, starty, 너비, 높이, hWnd, (HMENU)wParam번호, g_hInst, NULL)
		//푸시버튼
		CreateWindow(TEXT("button"), TEXT("상점"), WS_CHILD | WS_VISIBLE		//(TEXT("button"), TEXT(버튼내용), WS_CHILD | WS_VISIBLE
			| BS_PUSHBUTTON, 20, 50, 100, 25, hWnd, (HMENU)1, g_hInst, NULL);	//| BS_PUSHBUTTON, startx, starty, 너비, 높이, hWnd, (HMENU)wParam번호, g_hInst, NULL)
		CreateWindow(TEXT("button"), TEXT("격자눈금"), WS_CHILD | WS_VISIBLE		//(TEXT("button"), TEXT(버튼내용), WS_CHILD | WS_VISIBLE
			| BS_AUTOCHECKBOX, 20, 80, 100, 25, hWnd, (HMENU)2, g_hInst, NULL);	//| BS_PUSHBUTTON, startx, starty, 너비, 높이, hWnd, (HMENU)wParam번호, g_hInst, NULL)

		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))		//버튼의 wParam
		{
		case 0:
			ShowWindow(b, SW_SHOW);
			screen = 0;
			break;
		case 1:
			ShowWindow(b, SW_SHOW);
			screen = 1;
			break;
		case 2:
			if (isGrad) isGrad = FALSE;
			else isGrad = TRUE;
			break;
		default:
			break;
		}

	case WM_LBUTTONDOWN:
		if (screen == 0)
		{
			deck.DeckBuild(deck, mg.mx, mg.my);
		}
		else if (screen == 1)
		{
			shop.SelectChest(mg.mx, mg.my);

			/*if (InCircle(1217, 465, mg.mx, mg.my))
			{
				gacha.GetGacha(TRUE, deck);
			}
			else if (InCircle(1210, 565, mg.mx, mg.my))
			{
				gacha.GetGacha(FALSE, deck);
			}*/
		}

		InvalidateRect(hWnd, &rt, TRUE);
		return 0;

	case WM_MOUSEMOVE:
		mg.mx = LOWORD(lParam);
		mg.my = HIWORD(lParam);
		InvalidateRect(hWnd, &rt, TRUE);
		return 0;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		if (screen == 0)
		{
			deck.DrawDeckBuild(hdc, deck, hPen, oldPen, mg.mx, mg.my, print);
		}
		else if (screen == 1)
		{
			shop.DrawShop(hdc, hPen, oldPen, mg.mx, mg.my, print);
			gacha.DrawGacha(hdc, deck, hPen, oldPen, mg.mx, mg.my, print);
		}

		//변수 확인용
		wsprintf(print, TEXT(" : d %d"), deck.GetSize());
		TextOut(hdc, 10, 500, print, lstrlen(print));

		if (isGrad)
		{
			//드로우 확인용 100px간격 격자눈금
			for (int i = 0; i < 14; i++)
			{
				MoveToEx(hdc, (i + 1) * 100, rt.top, NULL);
				LineTo(hdc, (i + 1) * 100, rt.bottom);
			}
			for (int i = 0; i < 7; i++)
			{
				MoveToEx(hdc, rt.left, (i + 1) * 100, NULL);
				LineTo(hdc, rt.right, (i + 1) * 100);
			}
		}

		EndPaint(hWnd, &ps);
		return 0;
	}
	case WM_DESTROY: // 윈도우 종료 시(창 닫음 메시지)
		PostQuitMessage(0); // 메시지 큐에 종료 메시지 전달
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
