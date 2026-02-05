#include "CardManager.h"
#include "Card.h"
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"
#include "CardTableManager.h"
#include "Boss.h"
#include "Player.h"
#include "InputGame.h"

void CardManager::SetImage()
{
	M_REND.SetImage(L"background_city_night.png", "City_Night",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	M_REND.SetImage(L"background_pink_plant.png", "Plant_Pink",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	M_REND.SetImage(L"background_orange_plant.png", "Plant_Orange",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	M_REND.SetImage(L"background_dark_plant.png", "Plant_Dark",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), true,
		GameImage_M::LayerType::Background);
	M_REND.SetImage(L"background_ari_ship.png", "Air_Ship",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	M_REND.SetImage(L"card_zone.png", "Card_Middle_Up",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	M_REND.SetImage(L"card_zone.png", "Card_Middle_Down",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	M_REND.SetImage(L"card_back_2.png", "Card_Deck_Up",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	M_REND.SetImage(L"card_back.png", "Card_Deck_Down",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	for (size_t i = 0; i < 30; i++)
	{
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);

		M_REND.SetImage(L"card_back_2.png", cardId,
			Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
			GameImage_M::LayerType::Background);
	}

	cout << "이미지 로드 확인\n";
}

CardManager::CardManager() : m_DeckCount(25), m_HandCount(0), m_HandSelection(4),
m_IsMyTurn(false), m_IsSelect(false)
{
}

void CardManager::StartGame()
{
	CardDraw(5);
	cout << "처음 카드 드로우 확인\n";
}

CardManager::~CardManager()
{
	for (size_t i = 0; i < m_HandCount; i++)
	{
		delete m_Deck[i];
	}
}

int CardManager::GetDeckCount()
{
	return m_DeckCount;
}

int CardManager::GetHandCount()
{
	return m_HandCount;
}

vector<GameCard*> CardManager::GetHand()
{
	return m_Hand;
}

void CardManager::SetDeck()
{
	m_Deck = CardTableManager::Instance()->GetRandomCard(25);

	StartGame();
}

//드로우
void CardManager::CardDraw(int drawNum)
{
	for (size_t i = 0; i < drawNum; i++)
	{
		//덱에 카드가 없으면 리턴
		if (m_DeckCount <= 0)
			return;

		m_DeckCount--;
		m_HandCount++;


		m_Hand.push_back(m_Deck[m_DeckCount]);
		M_REND.ImageVisible(to_string(m_Deck[m_DeckCount]->GetUid()), true);
	}

	//패 카드 임시 확인
	cout << "패 카드 번호: [ ";
	for (size_t i = 0; i < m_HandCount; i++)
	{
		cout << m_Hand[i]->GetUid() << " ";
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
	M_REND.MoveImage("Card_Middle_Up",
		Gdiplus::Rect(midX - cardMidX, midY - (deckY + 10), deckX, deckY));
	M_REND.MoveImage("Card_Middle_Down",
		Gdiplus::Rect(midX - cardMidX, (midY + 10), deckX, deckY));

	M_REND.MoveImage("Card_Deck_Up",
		Gdiplus::Rect(0, 0, deckX, deckY));
	M_REND.MoveImage("Card_Deck_Down",
		Gdiplus::Rect(1265 - deckX, 682 - deckY, deckX, deckY));


	cout << "배경 출력 확인\n";
}

//플레이어 패 출력
void CardManager::DrawPlayerHand()
{
	//패가 없으면 리턴
	if (m_HandCount <= 0)
		return;

	int posY;
	posY = 720 - 180;

	int midX = 1280 * 0.5;
	int handMidX = midX - (CARDX * 2) - (CARDX * 0.5);
	//패 전체 길이는 임시로 카드 5장 길이로 설정
	int sliceHand = (CARDX * 5) / m_HandCount;

	//패가 5장 보다 적을 시
	if (m_HandCount < 5)
	{
		sliceHand = (CARDX * m_HandCount) / m_HandCount;
		handMidX = midX - 200;
	}

	for (size_t i = 0; i < m_HandCount; i++)
	{
		int startPos = handMidX + (sliceHand * i);
		if (i == m_HandSelection)
		{
			//카드 정보 확대 보기
			if (m_IsSelect)
			{
				M_REND.MoveImage(to_string(m_Hand[m_HandSelection]->GetUid()),
					Gdiplus::Rect(70, 380, CARDX * 2, CARDY * 2));
			}
			else
			{
				M_REND.MoveImage(to_string(m_Hand[i]->GetUid()),
					Gdiplus::Rect(startPos, posY - 10, CARDX, CARDY));
			}
		}
		else
		{
			M_REND.MoveImage(to_string(m_Hand[i]->GetUid()),
				Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
		}

		//카드 출력 순서
		M_REND.LayerMoveToBack(to_string(m_Hand[i]->GetUid()));
	}
}

//보스 패 출력
void CardManager::DrawOppHand()
{
	//패가 없으면 리턴
	if (m_HandCount <= 0)
		return;

	int posY;
	posY = 18;

	int midX = 1280 * 0.5;
	int handMidX = midX - (CARDX * 2) - (CARDX * 0.5);
	//패 전체 길이는 임시로 카드 5장 길이로 설정
	int sliceHand = (CARDX * 5) / m_HandCount;

	//패가 5장 보다 적을 시
	if (m_HandCount < 5)
	{
		sliceHand = (CARDX * m_HandCount) / m_HandCount;
		handMidX = midX - 200;
	}

	for (size_t i = 0; i < m_HandCount; i++)
	{
		int startPos = handMidX + (sliceHand * i);
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);
		M_REND.MoveImage(cardId,
			Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
	}
}

//패 카드 사용
void CardManager::CardAct(CardManager& player ,CardManager& opponent)
{
	//패에 카드가 없으면 리턴
	if (m_HandCount <= 0)
		return;
	//선택 중이지 않으면 리턴
	if (m_HandSelection < 0)
		return;
	
	//타이머 초기화
	m_timer.SetIsStart(false);

	switch (m_Hand[m_HandSelection]->GetType())
	{
	case E_Attack:
		cout << "공격 카드 사용!! " << m_Hand[m_HandSelection]->GetAtk() << "데미지!!\n";
		break;
	case E_Deffense:
		cout << "방어 카드 사용!!" << m_Hand[m_HandSelection]->GetDef() << "방어!!\n";
		break;
	case E_Magic:
		cout << "보조 카드 사용!!\n";
		break;
	}

	//이미지 안보이기
	M_REND.ImageVisible(to_string(m_Hand[m_HandSelection]->GetUid()), false);

	m_Hand.erase(m_Hand.begin() + m_HandSelection);
	m_HandCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (m_HandSelection >= m_HandCount && m_HandSelection != 0)
		m_HandSelection--;

	//턴 엔드
	player.m_IsMyTurn = !player.m_IsMyTurn;
	opponent.m_IsMyTurn = !opponent.m_IsMyTurn;
	opponent.BossCardAct(player);
	cout << "턴 교체.\n";
}

void CardManager::HandSelect(CardManager& player, CardManager& opponent)
{
	if (GameInput_M::Input::GetInstance().isKeyboard(KEY_LEFT)
		&& !(m_HandSelection <= 0)) m_HandSelection--;
	else if (GameInput_M::Input::GetInstance().isKeyboard(KEY_RIGHT)
		&& !(m_HandSelection >= m_HandCount - 1)) m_HandSelection++;

	else if (GameInput_M::Input::GetInstance().isKeyboard(KEY_UP)) m_IsSelect = true;
	else if (GameInput_M::Input::GetInstance().isKeyboard(KEY_DOWN)) m_IsSelect = false;
	else if (GameInput_M::Input::GetInstance().isKeyboard(KEY_ENTER) 
		&& m_IsMyTurn) CardAct(player, opponent);
}

//시작 턴 정하기
void CardManager::StartTurn(CardManager& player, CardManager& opponent)
{
	randomInit(0, 100);
	int randTurn;
	randTurn = cookRandom(gen);

	if (randTurn % 2 == 0)
	{
		player.m_IsMyTurn = !player.m_IsMyTurn;
		cout << "자신의 턴\n";
	}
	else
	{
		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;
		cout << "상대방의 턴\n";
		//opponent.OpponentAct(player, opponent, hWnd);
		//opponent.BossCardAct(player);
	}
}

void CardManager::TimeLimit(CardManager& player, CardManager& opponent)
{
	if (!m_timer.GetIsStart())
	{
		m_timer.StartTimer();
		m_timer.SetIsStart(true);
	}
	
	m_timer.UpdateTimer();
	if (m_timer.CheckTimer(5))
	{
		player.m_IsMyTurn = !player.m_IsMyTurn;
		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;

		//자신의 차례면 드로우
		if (player.m_IsMyTurn)
		{
			CardDraw(1);
			cout << "자신의 턴\n";
		}
		else
		{
			cout << "상대방의 턴\n";
			//opponent.BossCardAct(player);
		}
		m_timer.SetIsStart(false);
	}
}

//턴 시간 제한
//void CardManager::TimeLimit(WPARAM wParam, HWND hWnd, CardManager& player, CardManager& opponent)
//{
//	switch (wParam)
//	{
//	case TURNTIME:
//		//턴 엔드
//		player.m_IsMyTurn = !player.m_IsMyTurn;
//		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;
//
//		//자신의 차례면 드로우
//		if (player.m_IsMyTurn)
//		{
//			CardDraw(1);
//			cout << "자신의 턴\n";
//		}
//		else
//		{
//			cout << "상대방의 턴\n";
//			//opponent.OpponentAct(player, opponent, hWnd);
//		}
//
//		player.DrawPlayerHand();
//		opponent.DrawOppHand();
//		break;
//	default:
//		break;
//	}
//}

//보스 / 몬스터 행동
//void CardManager::OpponentAct(Player& p_player, Boss& p_boss, CardManager& player, CardManager& opponent, HWND hWnd)
//{
//	//드로우 
//	CardDraw(1);
//
//	//임시 정보 표출
//	string info;
//	for (int i = 0; i < m_HandCount; i++)
//	{
//		info += (to_string(i) + "번째 카드 정보: ");
//		info += (to_string(m_Hand[i]->GetStar() + 1) + "성. 공격력 " + to_string(m_Hand[i]->GetAtk())
//			+ ". 방어력 " + to_string(m_Hand[i]->GetDef()) + ".");
//		info += m_Hand[i]->GetInfo() + "\n";
//	}
//	cout << info << endl;
//	
//	//공격 카드로 플레이어를 죽일 수 있다면 1순위 액트
//	for (int i = 0; i < m_HandCount; i++)
//	{
//		if (m_Hand[i]->GetType() == E_Attack)
//		{
//			if (m_Hand[i]->GetAtk() >= p_player.GetHP())
//			{
//				m_HandSelection = i;
//				break;
//			}
//		}
//	}
//
//	
//	
//
//
//	BossCardAct(player, hWnd);
//}

void CardManager::BossCardAct(CardManager& player)
{
	//패에 카드가 없으면 리턴
	if (m_HandCount <= 0)
		return;
	//선택 중이지 않으면 리턴
	if (m_HandSelection < 0)
		return;

	switch (m_Hand[m_HandSelection]->GetType())
	{
	case E_Attack:
		cout << "공격 카드 사용!! " << m_Hand[m_HandSelection]->GetAtk() << "데미지!!\n";
		break;
	case E_Deffense:
		cout << "방어 카드 사용!!" << m_Hand[m_HandSelection]->GetDef() << "방어!!\n";
		break;
	case E_Magic:
		cout << "보조 카드 사용!!\n";
		break;
	}

	//이미지 안보이기
	M_REND.ImageVisible(to_string(m_Hand[m_HandSelection]->GetUid() + BOSSUID), false);

	m_Hand.erase(m_Hand.begin() + m_HandSelection);
	m_HandCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (m_HandSelection >= m_HandCount && m_HandSelection != 0)
		m_HandSelection--;

	//턴 엔드
	player.m_IsMyTurn = !player.m_IsMyTurn;
	m_IsMyTurn = !m_IsMyTurn;
	//SetTimer(hWnd, TURNTIME, 7000, NULL);
	player.CardDraw(1);
	cout << "턴 교체.\n";

}
