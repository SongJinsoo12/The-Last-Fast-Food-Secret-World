#include "Card.h"
#include <string>
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"
#include "ConstData.h"
#include "CardTableManager.h"

Card::Card()
{
	Init();
}

Card::Card(int p_uid)
{
	Init();
	uid = p_uid;
}

void Card::Init()
{
	uid = -1;
	atk = 0;
	def = 0;
	Ait = E_BREAD;
	Type = E_Attack;

	//SetImage();
}

int Card::GetUid()
{
	return uid;
}

void Card::SetUid(int p_uid)
{
	uid = p_uid;
}

int Card::GetAtk()
{
	return atk;
}

void Card::SetAtk(int p_atk)
{
	atk = p_atk;
}

int Card::GetDef()
{
	return def;
}

void Card::SetDef(int p_def)
{
	def = p_def;
}

CAttribute Card::GetAit()
{
	return Ait;
}

void Card::SetAit(CAttribute p_Ait)
{
	Ait = p_Ait;
}

CType Card::GetType()
{
	return Type;
}

void Card::SetType(CType p_Type)
{
	Type = p_Type;
}

void CardManager::SetImage()
{
	g_renderManager.SetImage(L"background_city_night.png", "City_Night",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720));

	g_renderManager.SetImage(L"card_zone.png", "Card_Middle_Up",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0));
	g_renderManager.SetImage(L"card_zone.png", "Card_Middle_Down",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0));

	g_renderManager.SetImage(L"card_back.png", "Card_Deck_Up",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0));
	g_renderManager.SetImage(L"card_back.png", "Card_Deck_Down",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0));

	

	cout << "이미지 로드 확인\n";
}

GameCard::GameCard()
{
}

GameCard::GameCard(Card* p_Card)
{
	this->SetUid(p_Card->GetUid());
	this->SetAtk(p_Card->GetAtk());
	this->SetDef(p_Card->GetDef());
	this->SetAit(p_Card->GetAit());
	this->SetType(p_Card->GetType());
}

GameCard::~GameCard()
{
}


CardManager::CardManager() : deckCount(25), handCount(0), handSelection(4), isMyTurn(false)
{
}

void CardManager::StartGame()
{
	CardDraw(5);
	cout << "처음 카드 드로우 확인\n";
}

CardManager::~CardManager()
{
	for (size_t i = 0; i < handCount; i++)
	{
		delete deck[i];
	}
}

int CardManager::GetDeckCount()
{
	return deckCount;
}

int CardManager::GetHandCount()
{
	return handCount;
}

vector<GameCard*> CardManager::GetHand()
{
	return hand;
}

void CardManager::SetDeck()
{
	deck = CardTableManager::Instance()->GetRandomCard(25);

	StartGame();
}

//드로우
void CardManager::CardDraw(int drawNum)
{
	for (size_t i = 0; i < drawNum; i++)
	{
		//덱에 카드가 없으면 리턴
		if (deckCount <= 0)
			return;

		deckCount--;
		handCount++;

		//임시
		hand.push_back(deck[deckCount - 1]);
	}

	//패 카드 임시 확인
	cout << "패 카드 번호: [ ";
	for (size_t i = 0; i < handCount; i++)
	{
		cout << hand[i]->GetUid() << " ";
	}
	cout << " ]\n";
}

//라인 그리기
void CardManager::DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY) {

	MoveToEx(hdc, startX, startY, nullptr);
	LineTo(hdc, lengthX, lengthY);
}

//배경 그리기
void CardManager::DrawBG()
{
	//화면 중간값 및 카드 길이 중간값
	int midX = 1280 * 0.5;
	int midY = 720 * 0.5;
	int cardMidX = CARDX * 0.5;
	int deckX = CARDX;
	int deckY = CARDY;

	/*g_renderManager.MoveImage("City_Night",
		Gdiplus::Rect(0, 0, 1280, 720));*/
	g_renderManager.MoveImage("Card_Middle_Up",
		Gdiplus::Rect(midX - cardMidX, midY - (deckY + 10), deckX, deckY));
	g_renderManager.MoveImage("Card_Middle_Down",
		Gdiplus::Rect(midX - cardMidX, (midY + 10), deckX, deckY));
	g_renderManager.MoveImage("Card_Deck_Up",
		Gdiplus::Rect(0, 0, deckX, deckY));
	g_renderManager.MoveImage("Card_Deck_Down",
		Gdiplus::Rect(1265 - deckX, 682 - deckY, deckX, deckY));
	

	cout << "배경 출력 확인\n";
}

//덱 장수 출력 
void CardManager::DrawDeckCount(HDC hdc, int rtX, int rtY, int cardX, int cardY)
{
	SetBkMode(hdc, TRANSPARENT); //문자 배경 투명
	SetTextColor(hdc, RGB(255, 255, 255)); //문자 색 변경
	TCHAR buffer[56];
	wsprintf(buffer, TEXT("%d"), this->GetDeckCount());
	TextOut(hdc, rtX + cardX, rtY + cardY, buffer, lstrlen(buffer));

	DrawCardInfo(hdc);
}

//패 출력
void CardManager::DrawHand(bool isPlayer)
{
	//패가 없으면 리턴
	if (handCount <= 0)
		return;

	int posY;
	if (isPlayer) 
		posY = 720 - 180;
	else 
		posY = 18;

	int midX = 1280 * 0.5;
	int handMidX = midX - (CARDX * 2) - (CARDX * 0.5);
	//패 전체 길이는 임시로 카드 5장 길이로 설정
	int sliceHand = (CARDX * 5) / handCount;

	//패가 5장 보다 적을 시
	if (handCount < 5)
	{
		sliceHand = (CARDX * handCount) / handCount;
		handMidX = midX - 200;
	}

	for (size_t i = 0; i < handCount; i++)
	{
		int startPos = handMidX + (sliceHand * i);
		if (i == handSelection && isPlayer)
		{
			g_renderManager.MoveImage(to_string(hand[i]->GetUid()), 
				Gdiplus::Rect(startPos, posY - 10, CARDX, CARDY));
		}
		else
		{
			g_renderManager.MoveImage(to_string(hand[i]->GetUid()), 
				Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
		}

	}
}

//선택 카드 정보 출력
void CardManager::DrawCardInfo(HDC hdc)
{
	SetBkMode(hdc, TRANSPARENT); //문자 배경 투명
	SetTextColor(hdc, RGB(0, 0, 0)); //문자 색 변경
	TCHAR info_uid[56];
	TCHAR info_atk_def[56];
	wsprintf(info_uid, TEXT("카드 번호: %d"), hand[handSelection]->GetUid());
	TextOut(hdc, 0, 400, info_uid, lstrlen(info_uid));
	
	switch (hand[handSelection]->GetAit())
	{
	case E_BULGOGI:
		TextOut(hdc, 0, 430, TEXT("카드 속성 : 불고기"), 26);
		break;
	case E_SOURCE:
		TextOut(hdc, 0, 430, TEXT("카드 속성 : 소스"), 23);
		break;
	case E_CHESSE:
		TextOut(hdc, 0, 430, TEXT("카드 속성 : 치즈"), 23);
		break;
	case E_VEGAT:
		TextOut(hdc, 0, 430, TEXT("카드 속성 : 야채"), 23);
		break;
	case E_BREAD:
		TextOut(hdc, 0, 430, TEXT("카드 속성 : 빵"), 20);
		break;
	}

	switch (hand[handSelection]->GetType())
	{
	case E_Attack:
		TextOut(hdc, 0, 460, TEXT("카드 타입 : 공격"), 23);

		wsprintf(info_atk_def, TEXT("카드 공격력: %d"), hand[handSelection]->GetAtk());
		TextOut(hdc, 0, 490, info_atk_def, lstrlen(info_atk_def));
		break;
	case E_Deffense:
		TextOut(hdc, 0, 460, TEXT("카드 타입 : 방어"), 23);

		wsprintf(info_atk_def, TEXT("카드 방어력: %d"), hand[handSelection]->GetDef());
		TextOut(hdc, 0, 490, info_atk_def, lstrlen(info_atk_def));
		break;
	case E_Magic:
		TextOut(hdc, 0, 460, TEXT("카드 타입 : 보조"), 23);

		TextOut(hdc, 0, 490, TEXT("보조 카드는 특별한 방법으로 플레이어를 돕습니다!"), 70);
		break;
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

	switch (hand[handSelection]->GetType())
	{
	case E_Attack:
		cout << "공격 카드 사용!!\n";
		break;
	case E_Deffense:
		cout << "방어 카드 사용!!\n";
		break;
	case E_Magic:
		cout << "보조 카드 사용!!\n";
		break;
	}

	//이미지 안보이기
	g_renderManager.RemoveIamage(to_string(hand[handSelection]->GetUid()));

	hand.erase(hand.begin() + handSelection);
	handCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (handSelection >= handCount && handSelection != 0)
		handSelection--;

	//턴 엔드
	this->isMyTurn = !this->isMyTurn;
	opponent.isMyTurn = !opponent.isMyTurn;
	SetTimer(hWnd, TURNTIME, 7000, NULL);
	cout << "상대방의 턴\n";

	opponent.OpponentAct();
}

//시작 턴 정하기
void CardManager::StartTurn(CardManager& player, CardManager& opponent)
{
	randomInit(0, 100);
	int randTurn;
	randTurn = cookRandom(gen);

	if (randTurn % 2 == 0)
	{
		player.isMyTurn = !player.isMyTurn;
		cout << "자신의 턴\n";
	}
	else
	{
		opponent.isMyTurn = !opponent.isMyTurn;
		cout << "상대방의 턴\n";
		opponent.OpponentAct();
	}

}

//턴 시간 제한
void CardManager::TimeLimit(WPARAM wParam, CardManager& opponent)
{
	switch (wParam)
	{
	case TURNTIME:
		//턴 엔드
		this->isMyTurn = !this->isMyTurn;
		opponent.isMyTurn = !opponent.isMyTurn;

		//자신의 차례면 드로우
		if (this->isMyTurn)
		{
			CardDraw(1);
			cout << "자신의 턴\n";
		}
		else
		{
			cout << "상대방의 턴\n";
			opponent.OpponentAct();
		}

		break;
	default:
		break;
	}
}

//보스 / 몬스터 행동
void CardManager::OpponentAct()
{
	CardDraw(1);
}
