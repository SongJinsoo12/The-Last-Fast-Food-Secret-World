#pragma once
#include <Windows.h>
#include <vector>

using namespace std;
class GameCard;
class Player;
class Mob;

#define LOAD 404

//덱 및 패 출력
class CardManager
{
public:
	CardManager();
	void StartGame();
	~CardManager();

	// ===== 테스트용 전투 스탯(간단 HP/방어막) =====
	int  GetHP() const;
	int  GetMaxHP() const;
	int  GetShield() const;
	void ResetBattleState(int maxHp = 30);

	// ===== 마우스 입력(플레이어 패 클릭) =====
	// (x,y)가 플레이어 패 영역의 카드에 닿으면 해당 카드를 즉시 사용합니다.
	// 성공적으로 처리했으면 true, 아니면 false를 반환합니다.
	bool HandlePlayerMouseClick(int x, int y, CardManager& opponent, HWND hWnd);
	int GetDeckCount();
	int GetHandCount();
	vector<GameCard*> GetHand();
	void SetDeck();
	void CardDraw(int drawNum);
	void CardAct(CardManager& opponent, HWND hWnd);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG();
	void DrawPlayerHand();
	void DrawOppHand();
	void HandSelect(WPARAM wParam, CardManager& opponent, HWND hWnd);
	void StartTurn(CardManager& player, CardManager& opponent);
	void TimeLimit(WPARAM wParam, CardManager& opponent);
	void OpponentAct();

	void SetImage();

	// 공격 2장 버리고 2턴 사용
	bool DiscardFirstAttackCard();

	// CardManager가 조작할 실제 전투 객체(Player/Mob) 연결
	void BindActors(Player* player, Mob* enemy);

	// ===== 디버그/테스트: 원하는 카드들을 패에 고정으로 세팅 =====
	// 랜덤 드로우 대신 특정 UID들을 패에 넣어서 하나씩 눌러 테스트할 수 있습니다.
// 기본: 200~233
// 기본: 1~40
// 기본: 101~120
	// 원하는 UID를 손패에 추가/교체 (디버그)
private:
	// UID 카드정보
	void ApplyUidMapping(GameCard* card);

	int m_DeckCount;//덱 장수
	int m_HandCount;//패 장수
	int m_HandSelection;//패 카드 선택
	vector<GameCard*> m_Hand;//패 카드
	vector<GameCard*> m_Deck;//덱 카드
	bool m_IsMyTurn;//턴 확인
	bool m_IsSelect;//패 카드 선택 확인

// ===== 보조카드/상태이상(테스트용) =====
int   m_TurnIndex = 0;            // 내 턴 카운터(턴 시작마다 +1)
float m_NextAtkMult = 1.0f;       // 다음 공격 배율(사용 후 1.0으로 리셋)
float m_DamageTakenMult = 1.0f;   // 받는 피해 배율(턴 수로 관리)
int   m_DamageTakenMultTurns = 0; // 남은 턴 수(0이면 비활성)
int   m_AtkProhibitTurns = 0;     // 공격 사용 금지 남은 턴 수

// 최근 사용 카드 기록(조건부 보조카드용)
//CType m_LastCardType = E_Attack;
int   m_LastAttackDamage = 0;

// 간단 지연 효과(회복/감소)
struct DelayedEffect
{
	int triggerTurn = 0; // m_TurnIndex 기준
	int heal = 0;        // 양수면 회복
	int hpLoss = 0;      // 양수면 감소
};
std::vector<DelayedEffect> m_Delayed;

// ===== 간단 전투 상태 =====
int m_HP;
int m_MaxHP; 
int m_Shield;

// ===== A안: DefCard 특수효과 상태 저장 =====
int  m_InvincibleHits = 0;        // uid142: 다음 공격 1회 무효
int  m_NextDamageReduction = 0;   // uid141: 다음 공격 피해 -N
int  m_ReflectDamage = 0;         // uid143: 다음에 피해를 받으면 공격자에게 반사
bool m_LastWasDefense = false;    // uid39용: 상대가 직전에 방어를 썼는지


// ===== 마우스 히트 테스트 =====
int HitTestPlayerHand(int x, int y) const;

// ===== 카드 타입별 효과(테스트용 기본 구현) =====
void ApplyAttackTo(CardManager& opponent, GameCard* card);
void ApplyDefense(GameCard* card, CardManager& opponent);
void ApplySupport(GameCard* card, CardManager& opponent);

// ===== uid40 DOT(출혈/독) =====
// 상대/내 턴이 끝날 때마다(턴 종료 시마다) 1회씩 발동
void AddDot(int dmgPerTick, int ticks); // ticks = '턴 종료 횟수' 기준
static void TickDotsAtTurnEnd(CardManager& a, CardManager& b);


// 보조카드
Player* m_pPlayer = nullptr;
Mob* m_pEnemyMob = nullptr;

// 다음 턴에 실드 초기호
bool m_ClearShieldAtNExtMyTurn = false;


// DOT 상태(uid40)
int m_DotDamagePerTick = 0;
int m_DotTicksRemaining = 0; // '턴 종료'마다 1씩 감소

// 카드 실행시키기위한 함수들
void ApplyDotDamageInternal(int dmg, Mob* realMob);

// 행도 카드 확인용
int m_PlayLimitThisTurn = 1;   // 이번 턴 사용 가능 횟수
int m_PlaysUsedThisTurn = 0;   // 이번 턴 사용한 횟수

void RefreshPlayLimitFromPlayer();
void TryEndTurn(CardManager& opponent, HWND hWnd);
};