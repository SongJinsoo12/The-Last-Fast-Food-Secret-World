#include "CardManager.h"
#include "Card.h"
#include "Player.h"
#include "Mob.h"
#include "SupportCard.h"
#include "AtkCard.h"
#include "DefCard.h"
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"
#include "CardTableManager.h"

#ifdef _DEBUG
#include <iostream>

static const char* TypeToStr(CType t)
{
	switch (t)
	{
	case E_Attack: return "Attack";
	case E_Deffense: return "Defense";
	case E_Magic: return "Magic";
	default: return "Unknown";
	}
}

static const char* AttrToStr(CAttribute a)
{
	switch (a)
	{
	case E_BULGOGI: return "BULGOGI";
	case E_SOURCE:  return "SOURCE";
	case E_CHESSE:  return "CHEESE";
	case E_VEGAT:return "VEGETABLE";
	case E_BREAD:   return "BREAD";
	default: return "UNK";
	}
}

static int StarToInt(Star s)
{
	switch (s)
	{
	case E_ONE: return 1;
	case E_TWO: return 2;
	case E_THREE: return 3;
	default: return 0;
	}
}

static void PrintCardDebug(const GameCard& c, const char* tag)
{
	std::cout << "[" << tag << "] "
		<< "UID=" << c.GetUid()
		<< " Type=" << TypeToStr(c.GetType())
		<< " Attr=" << AttrToStr(c.GetAit())
		<< " Star=" << StarToInt(c.GetStar())
		<< " ATK=" << c.GetAtk()
		<< " DEF=" << c.GetDef()
		<< "\n";
}
#endif

void CardManager::SetImage()
{
	m_rend.SetImage(L"background_city_night.png", "City_Night",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	m_rend.SetImage(L"background_pink_plant.png", "Plant_Pink",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	m_rend.SetImage(L"background_orange_plant.png", "Plant_Orange",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	m_rend.SetImage(L"background_dark_plant.png", "Plant_Dark",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), true,
		GameImage_M::LayerType::Background);
	m_rend.SetImage(L"background_ari_ship.png", "Air_Ship",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	m_rend.SetImage(L"card_zone.png", "Card_Middle_Up",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	m_rend.SetImage(L"card_zone.png", "Card_Middle_Down",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	m_rend.SetImage(L"card_back_2.png", "Card_Deck_Up",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	m_rend.SetImage(L"card_back.png", "Card_Deck_Down",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	for (size_t i = 0; i < 30; i++)
	{
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);

		m_rend.SetImage(L"card_back_2.png", cardId,
			Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
			GameImage_M::LayerType::Background);
	}

	cout << "이미지 로드 확인\n";
}

bool CardManager::DiscardFirstAttackCard()
{
	for (int i = 0; i < (int)m_Hand.size(); ++i)
	{
		if (m_Hand[i]->GetType() == CType::E_Attack)
		{
			// 버리기
			m_Hand.erase(m_Hand.begin() + i);

			// 카운트 갱신
			if (m_HandCount > 0)
			{
				m_HandCount--;
			}

			// 선택 인덱스
			if (m_HandSelection >= (int)m_Hand.size()) m_HandSelection = (int)m_Hand.size() - 1;

			return true;
		}
	}
	return false;
}

void CardManager::BindActors(Player* player, Mob* enemy)
{
	m_pPlayer = player;
	m_pEnemyMob = enemy;

	// (선택) UI용 HP를 Player 기준으로 초기 동기화
	if (m_pPlayer)
	{
		m_HP = m_pPlayer->GetHP();
		m_MaxHP = m_pPlayer->GetMaxHP();
	}
}

CardManager::CardManager() : m_DeckCount(25), m_HandCount(0), m_HandSelection(4),
m_IsMyTurn(false), m_IsSelect(false),
m_HP(30), m_MaxHP(30), m_Shield(0)
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
		m_rend.ImageVisible(to_string(m_Deck[m_DeckCount]->GetUid()), true);
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
	m_rend.MoveImage("Card_Middle_Up",
		Gdiplus::Rect(midX - cardMidX, midY - (deckY + 10), deckX, deckY));
	m_rend.MoveImage("Card_Middle_Down",
		Gdiplus::Rect(midX - cardMidX, (midY + 10), deckX, deckY));

	m_rend.MoveImage("Card_Deck_Up",
		Gdiplus::Rect(0, 0, deckX, deckY));
	m_rend.MoveImage("Card_Deck_Down",
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
				m_rend.MoveImage(to_string(m_Hand[m_HandSelection]->GetUid()),
					Gdiplus::Rect(70, 380, CARDX * 2, CARDY * 2));
			}
			else
			{
				m_rend.MoveImage(to_string(m_Hand[i]->GetUid()),
					Gdiplus::Rect(startPos, posY - 10, CARDX, CARDY));
			}
		}
		else
		{
			m_rend.MoveImage(to_string(m_Hand[i]->GetUid()),
				Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
		}

		//카드 출력 순서
		m_rend.LayerMoveToBack(to_string(m_Hand[i]->GetUid()));
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
		m_rend.MoveImage(cardId,
			Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
	}
}

//패 카드 선택
void CardManager::HandSelect(WPARAM wParam, CardManager& opponent, HWND hWnd)
{
	switch (wParam)
	{
	case VK_LEFT:
		if (m_HandSelection <= 0)
			return;
		m_HandSelection--;
		break;
	case VK_RIGHT:
		if (m_HandSelection >= m_HandCount - 1)
			return;
		m_HandSelection++;
		break;
		//임시 카드 정보 보기 버트
	case VK_UP:
		//CardInfo();
		m_IsSelect = true;
		break;
	case VK_DOWN:
		m_IsSelect = false;
		break;

		//임시 카드 내기 버튼
	case VK_RETURN:
		//자신의 턴이 아니면 행동 불가능
		if (!m_IsMyTurn)
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
	if (m_HandCount <= 0)
		return;
	//선택 중이지 않으면 리턴
	if (m_HandSelection < 0)
		return;

	/*ApplyUidMapping(m_Hand[m_HandSelection]);*/

	   // ★ 선택된 카드 포인터
	GameCard* sel = m_Hand[m_HandSelection];

#ifdef _DEBUG
	PrintCardDebug(sel, "BeforeMapping");   // (1) ApplyUidMapping 전 값
#endif

	ApplyUidMapping(sel);

#ifdef _DEBUG
	PrintCardDebug(sel, "AfterMapping");    // (2) ApplyUidMapping 후 값(덮였는지 확인)
	std::cout << "---- Switch Start ----\n";
#endif

	switch (m_Hand[m_HandSelection]->GetType())
	{
	case E_Attack:
		ApplyAttackTo(opponent, m_Hand[m_HandSelection]);
		cout << "공격 카드 사용!! " << m_Hand[m_HandSelection]->GetAtk() << "데미지!!\n";
		break;
	case E_Deffense:
		ApplyDefense(m_Hand[m_HandSelection], opponent);
		cout << "방어 카드 사용!! " << m_Hand[m_HandSelection]->GetDef() << "방어막!!\n";
		break;
	case E_Magic:
		ApplySupport(m_Hand[m_HandSelection], opponent);
		cout << "보조 카드 사용!!\n";
		// 보조카드가 Player/Mob에 영향을 줬을 수 있으니 UI 상태를 동기화
		if (m_pPlayer)
		{
			m_HP = m_pPlayer->GetHP();
			m_MaxHP = m_pPlayer->GetMaxHP();
		}
		if (m_pEnemyMob)
		{
			opponent.m_HP = m_pEnemyMob->GetHP();
			opponent.m_MaxHP = m_pEnemyMob->GetMaxHP();
		}
		break;

	}

	// A안: 상대가 직전에 방어카드를 썼는지 기록(uid39에서 사용)
	m_LastWasDefense = (sel && sel->GetType() == E_Deffense);

	//이미지 안보이기
	m_rend.ImageVisible(to_string(m_Hand[m_HandSelection]->GetUid()), false);

	m_Hand.erase(m_Hand.begin() + m_HandSelection);
	m_HandCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (m_HandSelection >= m_HandCount && m_HandSelection != 0)
		m_HandSelection--;

	//턴 엔드
	// uid40 DOT: 턴 종료마다(내턴/상대턴 끝날 때마다) 1회씩 발동
	// 이번 턴 사용 횟수 1 증가
	m_PlaysUsedThisTurn++;

	// 보조카드가 AddExtraPlaysThisTurn(2) 같은 걸 했을 수 있으니 최신화
	RefreshPlayLimitFromPlayer();

	// 사용 횟수 다 썼으면 턴 넘기기, 아니면 내 턴 유지
	TryEndTurn(opponent, hWnd);
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

		player.m_PlaysUsedThisTurn = 0;
		player.RefreshPlayLimitFromPlayer();
	}
	else
	{
		opponent.m_Shield = 0;
		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;
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
		// uid40 DOT: 턴 종료(시간초과)마다 1회씩 발동
		TickDotsAtTurnEnd(*this, opponent);

		//턴 엔드
		this->m_IsMyTurn = !this->m_IsMyTurn;
		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;

		//자신의 차례면 드로우
		if (this->m_IsMyTurn)
		{
			this->m_Shield = 0;
			CardDraw(1);
			cout << "자신의 턴\n";

			this->m_PlaysUsedThisTurn = 0;
			this->RefreshPlayLimitFromPlayer();
		}
		else
		{
			this->m_Shield = 0;
			cout << "상대방의 턴\n";
			opponent.OpponentAct();
		}

		this->DrawPlayerHand();
		opponent.DrawOppHand();
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
	* 보조 카드 우선 순위 ==> 1. 회복 2.
	*/

	CardDraw(1);

}

// =================== 테스트용 전투 스탯/마우스 입력 ===================

int CardManager::GetHP() const
{
	return m_HP;
}

int CardManager::GetMaxHP() const
{
	return m_MaxHP;
}

int CardManager::GetShield() const
{
	return m_Shield;
}

void CardManager::ResetBattleState(int maxHp)
{
	if (maxHp <= 0) maxHp = 1;
	m_MaxHP = maxHp;
	m_HP = maxHp;
	m_Shield = 0;
}

bool CardManager::HandlePlayerMouseClick(int x, int y, CardManager& opponent, HWND hWnd)
{
	// 내 턴이 아니면 클릭해도 사용 불가 (턴 룰 유지)
	if (!m_IsMyTurn)
		return false;

	int idx = HitTestPlayerHand(x, y);
	if (idx < 0 || idx >= m_HandCount)
		return false;

	m_HandSelection = idx;
	m_IsSelect = false;

	CardAct(opponent, hWnd);
	return true;
}

int CardManager::HitTestPlayerHand(int x, int y) const
{
	if (m_HandCount <= 0)
		return -1;

	// DrawPlayerHand()와 동일한 배치 로직 기반
	int posY = 720 - 180;

	int midX = (int)(1280 * 0.5);
	int handMidX = (int)(midX - (CARDX * 2) - (CARDX * 0.5));
	int sliceHand = (CARDX * 5) / m_HandCount;

	if (m_HandCount < 5)
	{
		sliceHand = CARDX;
		handMidX = midX - 200;
	}

	// y는 선택상태에 따라 +/-10 정도 흔들리므로 약간 여유
	int yTop = posY - 30;
	int yBottom = posY + CARDY + 30;

	if (y < yTop || y > yBottom)
		return -1;

	for (int i = 0; i < m_HandCount; ++i)
	{
		int startPos = handMidX + (sliceHand * i);
		int xLeft = startPos;
		int xRight = startPos + CARDX;

		if (x >= xLeft && x <= xRight)
			return i;
	}

	return -1;
}

// UID 카드 정보 저장
void CardManager::ApplyUidMapping(GameCard* card)
{
	if (!card) return;

	const int uid = card->GetUid();

	// 공격 카드: 1~40
	if (1 <= uid && uid <= 20)
	{
		card->SetAtk(5);
		card->SetDef(0);
		card->SetType(E_Attack);
		card->SetStar(E_ONE);
		if (uid <= 5) card->SetAit(E_BULGOGI);
		else if (uid <= 10) card->SetAit(E_SOURCE);
		else if (uid <= 15) card->SetAit(E_CHESSE);
		else card->SetAit(E_VEGAT);
		return;
	}

	if (21 <= uid && uid <= 32)
	{
		card->SetAtk(10);
		card->SetDef(0);
		card->SetType(E_Attack);
		card->SetStar(E_TWO);
		if (uid <= 23) card->SetAit(E_BULGOGI);
		else if (uid <= 26) card->SetAit(E_SOURCE);
		else if (uid <= 29) card->SetAit(E_CHESSE);
		else card->SetAit(E_VEGAT);
		return;
	}

	if (33 <= uid && uid <= 36)
	{
		card->SetAtk(15);
		card->SetDef(0);
		card->SetType(E_Attack);
		card->SetStar(E_THREE);
		switch (uid)
		{
		case 33: card->SetAit(E_BULGOGI); break;
		case 34: card->SetAit(E_SOURCE);  break;
		case 35: card->SetAit(E_CHESSE);  break;
		case 36: card->SetAit(E_VEGAT);   break;
		default: card->SetAit(E_BREAD);   break;
		}
		return;
	}

	// 빵 공격(표 그대로) // 특수 공격 카드 Atk 쪽에 구현 하게 있으니 그걸 사용
	if (uid == 37 || uid == 40)
	{
		card->SetAtk(0);
		card->SetDef(0);
		card->SetAit(E_BREAD);
		card->SetType(E_Attack);
		card->SetStar(E_TWO);
		return;
	}
	if (uid == 38 || uid == 39)
	{
		card->SetAtk(10);
		card->SetDef(0);
		card->SetAit(E_BREAD);
		card->SetType(E_Attack);
		card->SetStar(E_TWO);
		return;
	}

	// 방어 카드: 101~142
	if (101 <= uid && uid <= 120)
	{
		card->SetAtk(0);
		card->SetDef(5);
		card->SetType(E_Deffense);
		card->SetStar(E_ONE);
		if (uid <= 105) card->SetAit(E_BULGOGI);
		else if (uid <= 110) card->SetAit(E_SOURCE);
		else if (uid <= 115) card->SetAit(E_CHESSE);
		else card->SetAit(E_VEGAT);
		return;
	}

	if (121 <= uid && uid <= 132)
	{
		card->SetAtk(0);
		card->SetDef(10);
		card->SetType(E_Deffense);
		card->SetStar(E_TWO);
		if (uid <= 123) card->SetAit(E_BULGOGI);
		else if (uid <= 126) card->SetAit(E_SOURCE);
		else if (uid <= 129) card->SetAit(E_CHESSE);
		else card->SetAit(E_VEGAT);
		return;
	}

	if (133 <= uid && uid <= 136)
	{
		card->SetAtk(0);
		card->SetDef(15);
		card->SetType(E_Deffense);
		card->SetStar(E_THREE);
		switch (uid)
		{
		case 133: card->SetAit(E_BULGOGI); break;
		case 134: card->SetAit(E_SOURCE);  break;
		case 135: card->SetAit(E_CHESSE);  break;
		case 136: card->SetAit(E_VEGAT);   break;
		default:  card->SetAit(E_BREAD);   break;
		}
		return;
	}

	if (137 <= uid && uid <= 140)
	{
		card->SetAtk(0);
		card->SetDef(10);
		card->SetType(E_Deffense);
		card->SetStar(E_TWO);
		switch (uid)
		{
		case 137: card->SetAit(E_BULGOGI); break;
		case 138: card->SetAit(E_SOURCE);  break;
		case 139: card->SetAit(E_CHESSE);  break;
		case 140: card->SetAit(E_VEGAT);   break;
		default:  card->SetAit(E_BREAD);   break;
		}
		return;
	}
	// 이또한 특수 방어카드 Def에 구현 되어있으니 그걸사용
	if (uid == 141)
	{
		card->SetAtk(0);
		card->SetDef(10);
		card->SetAit(E_BREAD);
		card->SetType(E_Deffense);
		card->SetStar(E_TWO);
		return;
	}
	if (uid == 142)
	{
		card->SetAtk(0);
		card->SetDef(999);
		card->SetAit(E_BREAD);
		card->SetType(E_Deffense);
		card->SetStar(E_TWO);
		return;
	}

	// 보조 카드 : 200~233 (프로젝트 UID 범위)
	if (200 <= uid && uid <= 233)
	{
		card->SetAtk(0);
		card->SetDef(0);
		card->SetAit(E_BREAD);
		card->SetType(E_Magic);
		card->SetStar(E_TWO);
		return;
	}

	// 그 외 UID는 기존 카드 데이터 유지
}

void CardManager::ApplyAttackTo(CardManager& opponent, GameCard* card)
{
	if (!card) return;

	// -----------------------------
	// 0) 기본 데미지 준비
	// -----------------------------
	int dmg = card->GetAtk();
	if (dmg < 0) dmg = 0;

	// -----------------------------
	// 1) 방어 특수: 무적(uid142) 먼저 처리
	// -----------------------------
	if (opponent.m_InvincibleHits > 0)
	{
		opponent.m_InvincibleHits--;
		std::cout << "[DEF uid142] 무적 발동! 이번 공격 무효 (남은횟수=" << opponent.m_InvincibleHits << ")";
			return;
	}

	// -----------------------------
	// 2) AtkCard 특수 규칙 적용 (uid37~39)
	// -----------------------------
	// GameCard -> AtkCard로 복사해서 AtkCard.cpp의 함수들을 쓸 수 있게 함
	AtkCard atk;
	atk.SetUid(card->GetUid());
	atk.SetAtk(dmg);
	atk.SetDef(card->GetDef());
	atk.SetAit(card->GetAit());
	atk.SetType(card->GetType());
	atk.SetStar(card->GetStar());

	// uid37: "내 HP가 깎인 상태"면 보너스 데미지
	if (atk.GetUid() == 37)
	{
		bool iAmDamaged = (this->m_MaxHP > this->m_HP);
		if (iAmDamaged)
		{
			int bonus = 0;

			// AtkCard 함수 시그니처가 Mob&를 요구합니다.
			// 바인딩이 있으면 호출하고, 없으면 안전 계산으로 대체합니다.
			if (m_pEnemyMob)
				bonus = atk.Take_Damage_After_Atk(*m_pEnemyMob, atk.GetAit(), atk.GetStar());
			else
			{
				switch (atk.GetStar())
				{
				case E_ONE:   bonus = 2; break;
				case E_TWO:   bonus = 3; break;
				case E_THREE: bonus = 4; break;
				default:      bonus = 2; break;
				}
			}

			dmg += bonus;
			std::cout << "[ATK uid37] 보너스 +" << bonus << " => dmg=" << dmg << "";
		}
	}
	// uid38: 코인토스 공격(성공 시 데미지, 실패 시 0)
	else if (atk.GetUid() == 38)
	{
		int coin = atk.coinAtk(atk.GetAit(), atk.GetStar());
		dmg = coin;
		std::cout << "[ATK uid38] 코인 결과 dmg=" << dmg << "";
	}
	// uid39: 상대가 '직전에 방어카드'를 썼을 때만 데미지 발생
	else if (atk.GetUid() == 39)
	{
		bool defUsed = opponent.m_LastWasDefense;
		int out = atk.DefCard_After_Atk(defUsed, atk.GetStar());
		dmg = out;
		std::cout << "[ATK uid39] 상대방 직전 방어=" << defUsed << " => dmg=" << dmg << "";
	}

	// -----------------------------
	// 3) uid40(E_BREAD 공격): DOT(출혈/독) 부여
	// 요구사항: 내 턴/상대 턴이 끝날 때마다 1회씩 발동하도록 '턴 종료 틱'으로 관리
	// -----------------------------
	if (card->GetUid() == 40 && card->GetAit() == E_BREAD)
	{
		int dotDmg = 0;
		switch (card->GetStar())
		{
		case E_ONE:   dotDmg = 1; break;
		case E_TWO:   dotDmg = 2; break;
		case E_THREE: dotDmg = 3; break;
		default:      dotDmg = 1; break;
		}
		const int dotTicks = 6; // 3라운드(내턴끝+상대턴끝 = 2틱) => 6틱
		opponent.AddDot(dotDmg, dotTicks);
		std::cout << "[BREAD_ATK] uid40 DOT 부여: dmg=" << dotDmg << ", ticks=" << dotTicks << "";
	}

	// -----------------------------
	// 4) 방어 특수: 다음 피해 감소(uid141) 적용 (1회성)
	// -----------------------------
	if (opponent.m_NextDamageReduction > 0)
	{
		int before = dmg;
		dmg -= opponent.m_NextDamageReduction;
		if (dmg < 0) dmg = 0;
		std::cout << "[DEF uid141] 피해감소 -" << opponent.m_NextDamageReduction
			<< " (" << before << " -> " << dmg << ")";
			opponent.m_NextDamageReduction = 0;
	}

	// -----------------------------
	// 5) 방어막 우선 소모
	// -----------------------------
	int absorbed = (opponent.m_Shield < dmg) ? opponent.m_Shield : dmg;
	opponent.m_Shield -= absorbed;
	dmg -= absorbed;

	const int hpDmg = dmg; // 방어막 소모 후 실제 HP로 들어갈 데미지
	if (hpDmg > 0)
	{
		opponent.m_HP -= hpDmg;
		if (opponent.m_HP < 0) opponent.m_HP = 0;
	}

	// ✅ 실제 전투 객체(Mob)에도 반영(보조카드 로직과 HP 동기화를 위해)
	// 이 CardManager가 Player쪽일 때 m_pEnemyMob가 바인딩되어 있습니다.
	if (m_pEnemyMob && hpDmg > 0)
	{
		m_pEnemyMob->Damage(hpDmg);
		opponent.m_HP = m_pEnemyMob->GetHP();
		opponent.m_MaxHP = m_pEnemyMob->GetMaxHP();
	}

	// -----------------------------
	// 6) 방어 특수: 반사(uid143) 처리 (피해가 HP에 들어갔을 때 1회성)
	// -----------------------------
	if (hpDmg > 0 && opponent.m_ReflectDamage > 0)
	{
		int reflect = opponent.m_ReflectDamage;
		opponent.m_ReflectDamage = 0;

		std::cout << "[DEF uid143] 반사 피해 " << reflect << " 적용!";

			if (m_pPlayer)
			{
				m_pPlayer->Damage(reflect);
				m_HP = m_pPlayer->GetHP();
				m_MaxHP = m_pPlayer->GetMaxHP();
			}
			else
			{
				m_HP -= reflect;
				if (m_HP < 0) m_HP = 0;
			}
	}

	std::cout << "[전투] 공격 " << card->GetAtk()
		<< " (방어막 흡수 " << absorbed << ") -> 상대 HP " << opponent.m_HP
		<< ", 상대 방어막 " << opponent.m_Shield << "";
}

void CardManager::ApplyDefense(GameCard* card, CardManager& /*opponent*/)
{
	if (!card) return;

	// GameCard -> DefCard로 복사해서 DefCard.cpp 함수 사용
	DefCard def;
	def.SetUid(card->GetUid());
	def.SetAtk(card->GetAtk());
	def.SetDef(card->GetDef());
	def.SetAit(card->GetAit());
	def.SetType(card->GetType());
	def.SetStar(card->GetStar());

	// uid 141: 빵 방어(다음 공격 피해 감소)
	if (def.GetUid() == 141)
	{
		int reduce = def.BreadDef(E_BREAD, def.GetStar());
		m_NextDamageReduction = reduce;
		std::cout << "[DEF uid141] 다음 공격 피해 -" << reduce << " (1회)";
			return;
	}

	// uid 142: 무적(다음 공격 1회 무효)
	if (def.GetUid() == 142)
	{
		m_InvincibleHits = 1;
		std::cout << "[DEF uid142] 다음 공격 1회 무효(무적)";
			return;
	}

	// uid 143: 피해 반사(다음에 피해를 받으면 공격자에게 반사)
	if (def.GetUid() == 143)
	{
		int reflect = def.DamageReflection(def.GetAit(), def.GetStar());
		m_ReflectDamage = reflect;
		std::cout << "[DEF uid143] 다음 피해 발생 시 반사 " << reflect << " (1회)";
			return;
	}

	// 그 외 방어카드는 기존대로 "방어막 +" 처리
	int add = card->GetDef();
	if (add < 0) add = 0;

	m_Shield += add;
	m_ClearShieldAtNExtMyTurn = true;

	std::cout << "[전투] 방어막 +" << add
		<< " -> 내 HP " << m_HP << ", 내 방어막 " << m_Shield << "";
}

void CardManager::ApplySupport(GameCard* card, CardManager& opponent)

{
	if (!card) return;

	if (!m_pPlayer || !m_pEnemyMob)
	{
		std::cout << "[보조] 바인딩 없음: uid=" << card->GetUid() << " (효과 미적용)";
		return;
	}

	// =========================================================
	// (중요) \"보조카드 함수는 실행되는데 드로우/버리기가 안되는\" 이유
	// =========================================================
	// - SupportCard 로직은 Player/Mob 내부의 (vector<int>) 덱/패를 조작합니다.
	// - 하지만 현재 게임/UI는 CardManager의 m_Hand/m_Deck(GameCard*)를 \"진짜 패/덱\"처럼 사용합니다.
	// => 그래서 보조카드가 DiscardRandomHandCards(2) 같은 걸 해도 Player/Mob 쪽만 바뀌고,
	//    CardManager 화면/패(m_Hand)는 그대로라서 \"안 버려진 것처럼\" 보입니다.
	//
	// 해결: 보조카드 실행 전/후에 CardManager <-> Player/Mob 를 UID 기준으로 동기화합니다.

	auto ExtractHandIds = [](const std::vector<GameCard*>& v) -> std::vector<int>
		{
			std::vector<int> out;
			out.reserve(v.size());
			for (auto* c : v) out.push_back(c ? c->GetUid() : -1);
			return out;
		};

	auto ExtractDeckIdsRemaining = [](const std::vector<GameCard*>& deck, int deckCount) -> std::vector<int>
		{
			std::vector<int> out;
			if (deckCount < 0) deckCount = 0;
			if ((int)deck.size() < deckCount) deckCount = (int)deck.size();
			out.reserve(deckCount);
			for (int i = 0; i < deckCount; ++i)
				out.push_back(deck[i] ? deck[i]->GetUid() : -1);
			return out;
		};

	auto RebuildFromIds = [&](CardManager& cm, const std::vector<int>& deckIds, const std::vector<int>& handIds)
		{
			// 기존 손패 이미지 숨기기(유령 카드 방지)
			for (auto* c : cm.m_Hand)
				if (c) m_rend.ImageVisible(std::to_string(c->GetUid()), false);

			cm.m_Hand.clear();
			cm.m_Deck.clear();
			cm.m_HandCount = 0;
			cm.m_DeckCount = 0;
			cm.m_HandSelection = 0;

			// 덱(남은 카드들)
			for (int id : deckIds)
			{
				if (id <= 0) continue;
				Card* base = CardTableManager::Instance()->GetCardData(id);
				if (!base) continue;
				GameCard* gc = new GameCard(base);
				cm.ApplyUidMapping(gc);
				cm.m_Deck.push_back(gc);
				cm.m_DeckCount++;
			}

			// 패
			for (int id : handIds)
			{
				if (id <= 0) continue;
				Card* base = CardTableManager::Instance()->GetCardData(id);
				if (!base) continue;
				GameCard* gc = new GameCard(base);
				cm.ApplyUidMapping(gc);
				cm.m_Hand.push_back(gc);
				cm.m_HandCount++;
				m_rend.ImageVisible(std::to_string(id), true);
			}
		};

	// (1) CardManager -> Player/Mob 동기화
	m_pPlayer->Debug_SetHandIds(ExtractHandIds(m_Hand));
	m_pPlayer->Debug_SetDeckIds(ExtractDeckIdsRemaining(m_Deck, m_DeckCount));

	m_pEnemyMob->Debug_SetHandIds(ExtractHandIds(opponent.m_Hand));
	m_pEnemyMob->Debug_SetDeckIds(ExtractDeckIdsRemaining(opponent.m_Deck, opponent.m_DeckCount));

	// (2) SupportCard 실행
	SupportCard sc;
	sc.ApplyByUid(card->GetUid(), *m_pPlayer, *m_pEnemyMob, *this);

	// uid218은 SupportCard 내부에서 CardManager(cm) 손패를 직접 버립니다.
	// 이 경우 Player 손패(int id)에도 동일하게 반영해줘야 재동기화 시 카드가 되돌아오지 않습니다.
	if (card->GetUid() == 218)
	{
		m_pPlayer->Debug_SetHandIds(ExtractHandIds(m_Hand));
		m_pPlayer->Debug_SetDeckIds(ExtractDeckIdsRemaining(m_Deck, m_DeckCount));
	}

	// (3) Player/Mob -> CardManager 재동기화
	RebuildFromIds(*this, m_pPlayer->Debug_GetDeckIds(), m_pPlayer->Debug_GetHandIds());
	RebuildFromIds(opponent, m_pEnemyMob->Debug_GetDeckIds(), m_pEnemyMob->Debug_GetHandIds());

	// UI용 HP 동기화
	m_HP = m_pPlayer->GetHP();
	m_MaxHP = m_pPlayer->GetMaxHP();
	opponent.m_HP = m_pEnemyMob->GetHP();
	opponent.m_MaxHP = m_pEnemyMob->GetMaxHP();
}


void CardManager::AddDot(int dmgPerTick, int ticks)
{
	if (dmgPerTick < 0) dmgPerTick = 0;
	if (ticks < 0) ticks = 0;

	// 기존 DOT가 있으면 "더 강한 것/더 긴 것"으로 갱신(원하면 여기 규칙 바꾸면 됨)
	m_DotDamagePerTick = (dmgPerTick > m_DotDamagePerTick) ? dmgPerTick : m_DotDamagePerTick;
	m_DotTicksRemaining = (ticks > m_DotTicksRemaining) ? ticks : m_DotTicksRemaining;

	std::cout << "[DOT] 적용: dmg=" << m_DotDamagePerTick
		<< ", ticks=" << m_DotTicksRemaining << "\n";
}

void CardManager::TickDotsAtTurnEnd(CardManager& a, CardManager& b)
{
	// 규칙: "내 턴 종료" / "상대 턴 종료" 각각 끝날 때마다 DOT 1회씩 발동
	// => 턴 종료 이벤트마다 a,b 둘 다 한 번씩 틱을 돌립니다(둘 중 누구에게 DOT가 있든 상관없이)

	auto TickOne = [&](CardManager& target, Mob* realMob)
		{
			if (target.m_DotTicksRemaining <= 0 || target.m_DotDamagePerTick <= 0) return;

			const int dmg = target.m_DotDamagePerTick;
			target.m_DotTicksRemaining--;

			std::cout << "[DOT] 틱: dmg=" << dmg
				<< ", 남은ticks=" << target.m_DotTicksRemaining << "\n";

			// Player면 Player에, Mob이면 Mob에 반영
			target.ApplyDotDamageInternal(dmg, realMob);

			if (target.m_DotTicksRemaining == 0)
			{
				target.m_DotDamagePerTick = 0;
				std::cout << "[DOT] 종료\n";
			}
		};

	// a가 PlayerManager라면 a.m_pEnemyMob가 b(몹)일 가능성이 큼.
	// 반대로 b가 PlayerManager라면 b.m_pEnemyMob가 a(몹)일 가능성이 큼.
	Mob* mobForA = nullptr;
	Mob* mobForB = nullptr;

	if (!a.m_pPlayer && b.m_pEnemyMob) mobForA = b.m_pEnemyMob; // a가 몹뷰, b가 플레이어뷰
	if (!b.m_pPlayer && a.m_pEnemyMob) mobForB = a.m_pEnemyMob; // b가 몹뷰, a가 플레이어뷰

	TickOne(a, mobForA);
	TickOne(b, mobForB);
}

void CardManager::ApplyDotDamageInternal(int dmg, Mob* realMob)
{
	if (dmg <= 0) return;

	// 1) 플레이어가 바인딩된 매니저면 Player에 반영
	if (m_pPlayer)
	{
		m_pPlayer->Damage(dmg);
		m_HP = m_pPlayer->GetHP();
		m_MaxHP = m_pPlayer->GetMaxHP();
		return;
	}

	// 2) Mob 실체 포인터가 넘어오면 Mob에 반영 (상대/내턴 끝마다 DOT)
	if (realMob)
	{
		realMob->Damage(dmg);
		m_HP = realMob->GetHP();
		m_MaxHP = realMob->GetMaxHP();
		return;
	}

	// 3) 바인딩이 없으면 CardManager의 테스트용 HP만 깎기
	m_HP -= dmg;
	if (m_HP < 0) m_HP = 0;
}

void CardManager::RefreshPlayLimitFromPlayer()
{
	// Player가 바인딩되어 있으면 Player의 플레이 제한을 따라감
	if (m_pPlayer)
		m_PlayLimitThisTurn = m_pPlayer->GetPlayLimitThisTurn();
	else
		m_PlayLimitThisTurn = 1;
}

void CardManager::TryEndTurn(CardManager& opponent, HWND hWnd)
{
	// 아직 더 쓸 수 있으면 턴 안 넘김
	if (m_PlaysUsedThisTurn < m_PlayLimitThisTurn)
	{
		std::cout << "[Turn] plays: " << m_PlaysUsedThisTurn
			<< "/" << m_PlayLimitThisTurn << " (keep turn)\n";
		return;
	}

	// ✅ 여기부터가 “진짜 턴 종료”
	TickDotsAtTurnEnd(*this, opponent);

	this->m_IsMyTurn = !this->m_IsMyTurn;
	opponent.m_IsMyTurn = !opponent.m_IsMyTurn;

	// 상대 행동은 “즉시”가 아니라 타이머로 처리하고 싶으면 OpponentAct를 여기서 빼도 됨
	// 일단 기존 구조 유지 시:
	std::cout << "상대방의 턴\n";
	opponent.OpponentAct();
}

