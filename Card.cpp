#include "Card.h"

Card::Card() {
	Atk = 0;
	Rdc = 0;
	Ait = E_BREAD;
	Type = E_Attack;
	x = 0;
	y = 0;
}

CardManager::CardManager() : deckCount(25), handCount(5), handSelection(4), isMyTurn(false)
{
	Card temp;
	for (size_t i = 0; i < handCount; i++)
		hand.push_back(temp);
}

CardManager::~CardManager()
{
}

int CardManager::GetDeckCount()
{
	return deckCount;
}

int CardManager::GetHandCount()
{
	return handCount;
}

vector<Card> CardManager::GetHand()
{
	return hand;
}

//드로우
void CardManager::CardDraw()
{
	//덱에 카드가 없으면 리턴
	if (deckCount <= 0)
		return;

	deckCount--;
	handCount++;

	//임시
	Card temp; 
	hand.push_back(temp);
}

//라인 그리기
void CardManager::DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY) {

	MoveToEx(hdc, startX, startY, nullptr);
	LineTo(hdc, lengthX, lengthY);
}

//배경 그리기
void CardManager::DrawBG(HDC hdc, RECT rect, int cardX, int cardY)
{
	//화면 중간값 및 카드 길이 중간값
	int midX = rect.right * 0.5; 
	int midY = rect.bottom * 0.5;
	int cardMidX = cardX * 0.5;

	HPEN myPen, oldPen;
	myPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	oldPen = (HPEN)SelectObject(hdc, myPen);

	DrawLine(hdc, 0, midY, rect.right, midY);

	SelectObject(hdc, oldPen);
	DeleteObject(myPen);

	//중앙 카드 박스
	Rectangle(hdc, midX - cardMidX, midY - (cardY + 10), midX + cardMidX, midY - 10);
	Rectangle(hdc, midX - cardMidX, midY + 10, midX + cardMidX, midY + (cardY + 10));

	//덱 카드 박스
	HBRUSH myBrush, oldBrush;
	myBrush = CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = (HBRUSH)SelectObject(hdc, myBrush);
	Rectangle(hdc, 0, 0, cardX, cardY);
	Rectangle(hdc, rect.right - cardX, rect.bottom - cardY, rect.right, rect.bottom);

	SelectObject(hdc, oldBrush);
	DeleteObject(myBrush);

	//패에 카드 없으면 리턴
	if (handCount <= 0)
		return;

	//선택 중인 패 카드 정보 출력
	myPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	oldPen = (HPEN)SelectObject(hdc, myPen);
	Rectangle(hdc, rect.left + 30, midY + 30, (rect.left + 30) + (cardX * 3), (midY + 30) + (cardY * 3));
	SelectObject(hdc, oldPen);
	DeleteObject(myPen);
}

//덱 장수 출력 
void CardManager::DrawDeckCount(HDC hdc, int rtX, int rtY, int cardX, int cardY)
{
	SetBkMode(hdc, TRANSPARENT); //문자 배경 투명
	SetTextColor(hdc, RGB(255, 255, 255)); //문자 색 변경
	TCHAR buffer[56];
	wsprintf(buffer, TEXT("%d"), this->GetDeckCount());
	TextOut(hdc, rtX + cardX, rtY + cardY, buffer, lstrlen(buffer));
}

//패 출력
void CardManager::DrawHand(HDC hdc, int rtX, int rtY, int cardX, int cardY, bool isPlayer)
{
	//패가 없으면 리턴
	if (handCount <= 0)
		return;

	int midX = rtX * 0.5;
	int handMidX = midX - (cardX * 2) - (cardX * 0.5); 
	//패 전체 길이는 임시로 카드 5장 길이로 설정
	int sliceHand = (cardX * 5) / handCount; 

	//패가 5장 보다 적을 시
	if (handCount < 5)
	{
		sliceHand = (cardX * handCount) / handCount;
		handMidX = midX - (cardX);
	}

	for (size_t i = 0; i < handCount; i++)
	{
		int startPos = handMidX + (sliceHand * i);
		if (i == handSelection && isPlayer)
		{
			HPEN myPen, oldPen;
			myPen = CreatePen(PS_SOLID, 1, RGB(255, 215, 0));
			oldPen = (HPEN)SelectObject(hdc, myPen);
			Rectangle(hdc, startPos, rtY - 10, startPos + cardX, rtY + (cardY - 10));
			SelectObject(hdc, oldPen);
			DeleteObject(myPen);
		}
		else
		{
			Rectangle(hdc, startPos, rtY + 10, startPos + cardX, rtY + (10 + cardY));
		}
	}
}

//패 카드 선택
void CardManager::HandSelect(WPARAM wParam, CardManager& opponent, HWND hWnd)
{
	switch (wParam)
	{
	case VK_LEFT:
		if (handSelection <= 0)
			return;
		handSelection--;
		break;
	case VK_RIGHT:
		if (handSelection >= handCount - 1)
			return;
		handSelection++;
		break;
	//임시 카드 내기 버튼
	case VK_RETURN:
		//자신의 턴이 아니면 행동 불가능
		if (!isMyTurn)
			return;
		CardAct(opponent, hWnd);
		break;
	//임시 카드 드로우 버튼
	case VK_DOWN:
		break;
	default:
		break;
	}
}

//패 카드 사용
void CardManager::CardAct(CardManager& opponent, HWND hWnd)
{
	//패에 카드가 없으면 리턴
	if (handCount <= 0)
		return;
	//선택 중이지 않으면 리턴
	if (handSelection < 0)
		return;

	hand.erase(hand.begin() + handSelection);
	handCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (handSelection >= handCount && handSelection != 0)
		handSelection--;

	//턴 엔드
	this->isMyTurn = !this->isMyTurn;
	opponent.isMyTurn = !opponent.isMyTurn;
	SetTimer(hWnd, TURNTIME, 7000, NULL);
}

//시작 턴 정하기
void CardManager::StartTurn(CardManager& player, CardManager& opponent)
{
	randomInit(0, 100);
	int randTurn;
	randTurn = cookRandom(gen);

	if (randTurn % 2 == 0)
		player.isMyTurn = !player.isMyTurn;
	else
		opponent.isMyTurn = !opponent.isMyTurn;
}

//턴 시간 제한
void CardManager::TimeLimit(WPARAM wParam, CardManager& opponent, HWND hWnd, LPCWSTR text, LPCWSTR caption, UINT type)
{
	switch (wParam)
	{
	case TURNTIME:
		//임시 메시지 박스
		MessageBox(hWnd, text, caption, type);

		//턴 엔드
		this->isMyTurn = !this->isMyTurn;
		opponent.isMyTurn = !opponent.isMyTurn;

		//자신의 차례면 드로우
		if (this->isMyTurn)
			CardDraw();
		break;
	default:
		break;
	}
}
