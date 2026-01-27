#include <windows.h>										// 헤더
#include "Card.h"
#include "CardManager.h"
#include "CardTableManager.h"
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"


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
		WS_OVERLAPPEDWINDOW, /*CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT*/
		0, 0, 1280, 720, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) { //Queue에 있는 메시지 읽어들임
		TranslateMessage(&Message); // 키보드 입력 메시지 가공
		DispatchMessage(&Message); // 메시지 처리
	}
	return (int)Message.wParam; // 탈출 코드, 프로그램 종료
}



CardManager g_player, g_enemy;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc, memDC;
	PAINTSTRUCT ps;
	HBITMAP hBitmap,hOldBitmap;
	RECT rt;

	switch (iMessage) {
	case WM_CREATE:
	{
		GetClientRect(hWnd, &rt);
		g_player.SetImage();
		g_player.DrawBG();

		//임시 덱 로드
		CardTableManager::Instance();

		cout << "로딩 중...\n";
		SetTimer(hWnd, 404, 3000, NULL);

		return 0;
	}

	case WM_TIMER:
		switch (wParam)
		{
		case LOAD:
			g_player.SetDeck();
			g_enemy.SetDeck();

			g_player.StartTurn(g_player, g_enemy);
			g_player.DrawPlayerHand();
			g_enemy.DrawOppHand();
			SetTimer(hWnd, TURNTIME, 7000, NULL);
			KillTimer(hWnd, LOAD);
			break;
		}

		g_player.TimeLimit(wParam, g_enemy);
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;

	case WM_KEYDOWN:
		g_player.HandSelect(wParam, g_enemy, hWnd);
		g_player.DrawPlayerHand();
		g_enemy.DrawOppHand();
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &rt);

		memDC = CreateCompatibleDC(hdc);
		hBitmap = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
		hOldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

		PatBlt(memDC, 0, 0, rt.right, rt.bottom, WHITENESS);

		Graphics graphics(memDC);
		graphics.Clear(Color(255, 255, 255, 255));

		/*g_player.DrawPlayerHand();
		g_enemy.DrawOppHand();*/

		m_rend.RenderAll(&graphics);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, memDC, 0, 0, SRCCOPY);

		SelectObject(memDC, hOldBitmap);
		DeleteObject(hBitmap);
		DeleteDC(memDC);
		EndPaint(hWnd, &ps);
		return 0;
	}

	case WM_DESTROY: // 윈도우 종료 시(창 닫음 메시지)
		PostQuitMessage(0); // 메시지 큐에 종료 메시지 전달
		KillTimer(hWnd, TURNTIME);
		return 0;

	case WM_ERASEBKGND:
		return 1;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}