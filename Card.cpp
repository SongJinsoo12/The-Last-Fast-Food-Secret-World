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
	star = E_TWO;
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

Star Card::GetStar()
{
	return star;
}

void Card::SetStar(Star p_Star)
{
	star = p_Star;
}

void CardManager::SetImage()
{
	g_renderManager.SetImage(L"background_city_night.png", "City_Night",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720));

	g_renderManager.SetImage(L"card_zone.png", "Card_Middle_Up",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0));
	g_renderManager.SetImage(L"card_zone.png", "Card_Middle_Down",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0));

	g_renderManager.SetImage(L"card_back_2.png", "Card_Deck_Up",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0));
	g_renderManager.SetImage(L"card_back.png", "Card_Deck_Down",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0));

	for (size_t i = 0; i < 30; i++)
	{
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);

		g_renderManager.SetImage(L"card_back_2.png", cardId,
			Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0));
	}

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


CardManager::CardManager() : deckCount(25), handCount(0), handSelection(4), 
isMyTurn(false), isSelect(false)
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
		hand.push_back(deck[deckCount]);
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

//플레이어 패 출력
void CardManager::DrawPlayerHand()
{
	//패가 없으면 리턴
	if (handCount <= 0)
		return;

	int posY;
	posY = 720 - 180;

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
		if (i == handSelection)
		{
			//카드 정보 확대 보기
			if (isSelect)
			{
				g_renderManager.MoveImage(to_string(hand[handSelection]->GetUid()),
					Gdiplus::Rect(70, 380, CARDX * 2, CARDY * 2));
			}
			else
			{
				g_renderManager.MoveImage(to_string(hand[i]->GetUid()),
					Gdiplus::Rect(startPos, posY - 10, CARDX, CARDY));
			}
		}
		else
		{
			g_renderManager.MoveImage(to_string(hand[i]->GetUid()),
				Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
		}
	}
}

//보스 패 출력
void CardManager::DrawOppHand()
{
	//패가 없으면 리턴
	if (handCount <= 0)
		return;

	int posY;
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
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);
		g_renderManager.MoveImage(cardId,
			Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
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
		//임시 카드 정보 보기 버트
	case VK_UP:
		//CardInfo();
		isSelect = true;
		break;
	case VK_DOWN:
		isSelect = false;
		break;

		//임시 카드 내기 버튼
	case VK_RETURN:
		//자신의 턴이 아니면 행동 불가능
		if (!isMyTurn)
			return;
		CardAct(opponent, hWnd);
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
	/*
	* 1. 카드 드로우
	* 2. hp가 절반 이상일 경우 --> 공격 카드 서칭 --> 가장 공격력이 높은 카드 사용
	*						--> 공격 카드가 없을 경우 --> 방어 카드 서칭 --> 가장 방어력 높은 카드 사용
	*						--> 방어 카드 없을 경우 --> 보조 카드 사용
	* 3. hp가 절반 이하일 경우 --> 보조 카드 중 회복 카드 서칭 --> 회복력 가장 높은 카드 사용
	*						--> 회복 카드 없을 경우 방어 카드 서칭 --> 가장 방어력 높은 카드 사용
	*						--> 방어 카드 없을 경우 공격 카드 서칭 --> 가장 공격력이 높은 카드 사용
	*						--> 공격 카드 없을 경우 보조 카드 사용
	* 
	* 공격 카드 우선 순위 ==> 1. 공격력 2. 
	* 방어 카드 우선 순위 ==> 1. 방어력 2. 
	* 보조 카드 우선 순위 ==> 얘네가 진짜 ㅅㅂ럼들임 ㅇㅇ. 1. 회복 2. 
	*/

	CardDraw(1);


}
