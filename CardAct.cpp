#include "card_act.h"

//라인 그리기
void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY) {

	MoveToEx(hdc, startX, startY, nullptr);
	LineTo(hdc, lengthX, lengthY);
}

//배경 그리기
void DrawBG(HDC hdc, RECT rect)
{
	int midX = rect.right / 2;
	int midY = rect.bottom / 2;

	HPEN myPen, oldPen;
	myPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	oldPen = (HPEN)SelectObject(hdc, myPen);

	DrawLine(hdc, 0, midY, rect.right, midY);

	SelectObject(hdc, oldPen);
	DeleteObject(myPen);
	
	//중앙 카드 박스
	Rectangle(hdc, midX - 61, midY - 200, midX + 61, midY - 29);
	Rectangle(hdc, midX - 61, midY + 29, midX + 61, midY + 200);

	//덱 카드 박스
	HBRUSH myBrush, oldBrush;
	myBrush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	Rectangle(hdc, 0, 0, 122, 171);
	Rectangle(hdc, rect.right - 122, rect.bottom - 171, rect.right, rect.bottom); 

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);
}

//덱 장수 출력
void DrawDeckCount(HDC hdc, Deck& playerHand, Deck& enemyHand, RECT rect)
{
	SetBkMode(hdc, TRANSPARENT); //문자 배경 투명
	SetTextColor(hdc, RGB(255, 255, 255)); //문자 색 변경
	TCHAR buffer[56];
	TCHAR buffer_2[56];
	wsprintf(buffer, TEXT("%d"), playerHand.GetDeckCount());
	wsprintf(buffer_2, TEXT("%d"), enemyHand.GetDeckCount());
	TextOut(hdc, rect.right - 60, rect.bottom - 85, buffer, lstrlen(buffer));
	TextOut(hdc, rect.left + 55, rect.top + 85, buffer_2, lstrlen(buffer_2));
}
