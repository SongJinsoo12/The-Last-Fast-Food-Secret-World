#include <windows.h>
<<<<<<< HEAD
#include <iostream>// 
=======
#include <iostream>// 헤더
>>>>>>> 41382111320f47a0c72622284717d593348aec06
//#include "tweeny.h"
#include "Mob.h"
#include "Player.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
<<<<<<< HEAD
HINSTANCE g_hInst;											// νϽ ڵ
LPCTSTR lpszClass = TEXT("T.L.F.F2 : Secret World");					//  ǥٿ ǥ
=======
HINSTANCE g_hInst;											// 인스턴스 핸들
LPCTSTR lpszClass = TEXT("T.L.F.F2 : Secret World");					// 제목 표시줄에 표시
>>>>>>> 41382111320f47a0c72622284717d593348aec06

//auto tween = tweeny::from(50).to(500).during(2000).via(tweeny::easing::exponentialOut);
//
//LARGE_INTEGER frequency, t1, t2;
//double deltaTime = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
#ifdef _DEBUG
	if (AllocConsole()) {
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
<<<<<<< HEAD
		freopen_s(&fp, "CONOUT$", "w", stderr); //  µ ַܼ
		std::ios::sync_with_stdio(); // cout printf    
	}
#endif

	HWND hWnd;												//  ڵ 
	MSG Message;											// ޽ ü  
	WNDCLASS WndClass;										// Windows Class ü  
	g_hInst = hInstance;									// hInstance ܺο ǰ   

	WndClass.cbClsExtra = 0;								//  ,   x
	WndClass.cbWndExtra = 0;								//  
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//   
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);			// 콺   
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);		// Ŀ  
	WndClass.hInstance = hInstance;							// Ŭ ϴ α׷ ȣ
	WndClass.lpfnWndProc = WndProc;							// ޽ ó Լ 
	WndClass.lpszClassName = lpszClass;						// Ŭ ̸ 
	WndClass.lpszMenuName = NULL;							// ޴ 
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;		// Ÿ 

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, //  
=======
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
>>>>>>> 41382111320f47a0c72622284717d593348aec06
		WS_OVERLAPPEDWINDOW, /*CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT*/
		100,0,1280,720, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

<<<<<<< HEAD
	while (true) { //Queue ִ ޽ о
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message); // Ű Է ޽ 
			DispatchMessage(&Message); // ޽ ó
		}
		else {
			// 0. Ÿ̸ ó
=======
	while (true) { //Queue에 있는 메시지 읽어들임
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) {
			if (Message.message == WM_QUIT)
				break;
			TranslateMessage(&Message); // 키보드 입력 메시지 가공
			DispatchMessage(&Message); // 메시지 처리
		}
		else {
			// 0. 타이머 처리
>>>>>>> 41382111320f47a0c72622284717d593348aec06
			// 
			//QueryPerformanceCounter(&t2);
			//deltaTime = (double)(t2.QuadPart - t1.QuadPart) / frequency.QuadPart;
			//t1 = t2;
<<<<<<< HEAD
			// 1.  ġ Ʈ (̵, 浹 )
			// GameInput_M::Input::GetInstance().Update();
			// 
			// Render ڸ
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	return (int)Message.wParam; // Ż ڵ, α׷ 
=======
			// 1. 게임의 수치들을 업데이트 (이동, 충돌 등)
			// GameInput_M::Input::GetInstance().Update();
			// 
			// Render 자리
			InvalidateRect(hWnd, NULL, FALSE);
		}
	}
	return (int)Message.wParam; // 탈출 코드, 프로그램 종료
>>>>>>> 41382111320f47a0c72622284717d593348aec06
}

#include <math.h>
#include "RenderManager.h"
#include "GameState.h"
#include "InputGame.h"
#include "ButtonManager.h"


// ------------------------------------------------------------
// HUD: 화면 좌측 상단에 HP/Shield만 2줄 표시
// ------------------------------------------------------------

Player g_playerActor;
Mob    g_enemyActor;
CardManager g_player;
CardManager g_enemy;

static void DrawSimpleHUD(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	const int pHP = g_playerActor.GetHP();
	const int pMax = g_playerActor.GetMaxHP();
	const int pSh = g_playerActor.GetShield();

	const int eHP = g_enemyActor.GetHP();
	const int eMax = g_enemyActor.GetMaxHP();
	const int eSh = g_enemyActor.GetShield();

	wchar_t line1[128], line2[128];
	wsprintfW(line1, L"PLAYER  HP %d/%d   SH %d", pHP, pMax, pSh);
	wsprintfW(line2, L"ENEMY   HP %d/%d   SH %d", eHP, eMax, eSh);

	TextOutW(hdc, 20, 20, line1, lstrlenW(line1));
	TextOutW(hdc, 20, 45, line2, lstrlenW(line2));
}

//#define BSIZE 40
//
//double LengthPts(int x1, int y1, int x2, int y2) {
//	return(sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1))));
//}
//
//BOOL InCircle(int x, int y, int mx, int my) {
//	if (LengthPts(x, y, mx, my) < BSIZE)return TRUE;
//	else return FALSE;
//}

//  ü

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static RECT rt;
	HDC memDC;
	HBITMAP hOldBitmap;

	GameInput_M::Input::GetInstance().UpdateProcess(hWnd, iMessage, wParam, lParam);

	switch (iMessage) {
	case WM_CREATE:
		GetClientRect(hWnd, &rt);
<<<<<<< HEAD
		//
		STATE.ChangeState(GameState_M::E_InGameState::Lobby);

		// Actor(HP/Shield) 연결: 카드 효과가 실제 HP/Shield에 적용되게 합니다.
		g_player.BindActors(&g_playerActor, &g_enemyActor);
		g_enemy.BindActors(nullptr, &g_enemyActor);

=======
		GameState_M::Context::GetInstance().Init();
		GameState_M::Context::GetInstance().ChangeState(GameState_M::E_InGameState::Lobby);
>>>>>>> 41382111320f47a0c72622284717d593348aec06
		//SetTimer(hWnd, 1, 500, NULL);
		break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);

<<<<<<< HEAD
		// 1. ޸ DC Ʈ  ( ۸)
		memDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(memDC, CreateCompatibleBitmap(hdc, rt.right, rt.bottom));

		// 2. ޸ DC  Graphics 
		Graphics graphics(memDC);
		graphics.Clear(Color(255, 255, 255, 255)); //  

		// 3.  ̹ ׸
		STATE.Update(hdc, hWnd);
=======
		// 1. 메모리 DC와 비트맵 생성 (더블 버퍼링용)
		memDC = CreateCompatibleDC(hdc);
		hOldBitmap = (HBITMAP)SelectObject(memDC, CreateCompatibleBitmap(hdc, rt.right, rt.bottom));

		// 2. 메모리 DC 위에 Graphics 생성
		Graphics graphics(memDC);
		graphics.Clear(Color(255, 255, 255, 255)); // 배경 지우기

		// 3. 모든 이미지 그리기
		GameState_M::Context::GetInstance().Update(hdc, hWnd);
>>>>>>> 41382111320f47a0c72622284717d593348aec06
		GameImage_M::RenderManager::GetInstance().RenderAll(&graphics);
		btnManager.DrawAll();

<<<<<<< HEAD
		// 4. ޸𸮿 ׸   ȭ    ( )
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, memDC, 0, 0, SRCCOPY);

		// 5. ޸ 
=======
		// 4. 메모리에 그린 내용을 실제 화면으로 한 번에 복사 (깜빡임 방지)
		BitBlt(hdc, 0, 0, rt.right, rt.bottom, memDC, 0, 0, SRCCOPY);

		// 5. 메모리 해제
>>>>>>> 41382111320f47a0c72622284717d593348aec06
		DeleteObject(SelectObject(memDC, hOldBitmap));
		DeleteDC(memDC);
		DrawSimpleHUD(hdc);

		EndPaint(hWnd, &ps);
	}
	return 0;

<<<<<<< HEAD
	case WM_DESTROY: //   (â  ޽)
		PostQuitMessage(0); // ޽ ť  ޽ 
=======
	case WM_DESTROY: // 윈도우 종료 시(창 닫음 메시지)
		PostQuitMessage(0); // 메시지 큐에 종료 메시지 전달
>>>>>>> 41382111320f47a0c72622284717d593348aec06
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
