#pragma once
#include "Timer.h"
#include "Animation.h"
#include <Windows.h>
#include <vector>
#include "Card.h"


using namespace std;
//class GameCard;
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

	int GetDeckCount();
	int GetHandCount();
	vector<GameCard*> GetHand();
	void SetDeck();
	void CardDraw(int drawNum);
	void CardAct(CardManager& player, CardManager& opponent);

	void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
	void DrawBG();
	void DrawPlayerHand();
	void DrawOppHand();
	void HandSelect(CardManager& player, CardManager& opponent);
	void StartTurn(CardManager& player, CardManager& opponent);
	void TimeLimit(CardManager& player, CardManager& opponent);
	void BossCardAct(CardManager& player);
	void SetImage();
	void LoadMonoGram(string p_image, string p_id, int p_x, int p_y);
	void RemoveMonoGram(string p_id);
	void PuaseGame();
	
	bool* GetIsShiny();
	bool* GetIsRip();
	bool* GetIsSkill();
	bool* GetIsDef();


	//void SetPlayer(Player* pPlayer);
	//void SetEnemyMob(Mob* pMob);

	// 공격 2장 버리고 2턴 사용
	bool DiscardFirstAttackCard();

	// CardManager가 조작할 실제 전투 객체(Player/Mob) 연결
	void BindActors(Player* player, Mob* enemy);

	// 턴관련
	bool IsMyTurn() const { return m_IsMyTurn; }
	void SetMyTurn(bool v) { m_IsMyTurn = v; } // 필요하면

private:


	bool m_isShiny, m_isRip, m_isSkill, m_isDef;
	int m_monoId;
	// UID 카드정보
	

	int m_DeckCount;//덱 장수
	int m_HandCount;//패 장수
	int m_HandSelection;//패 카드 선택
	vector<GameCard*> m_Hand;//패 카드
	vector<GameCard*> m_Deck;//덱 카드
	bool m_IsMyTurn;//턴 확인
	bool m_IsSelect;//패 카드 선택 확인
	//AI m_boss;//보스 ai
	Timer m_timer, m_shiny, m_rip;

	// ===== 카드 타입별 효과(테스트용 기본 구현) =====
	void ApplyAttackTo(CardManager& opponent, GameCard* card);
	void ApplyDefense(GameCard* card);
	void ApplySupport(GameCard* card, CardManager& opponent);

	// player랑 Mob(Boss) 가져오기
	Player* m_pPlayer = nullptr;
	Mob* m_pEnemyMob = nullptr;

	// 헬퍼 추가
	int  ActorHP() const;
	int  ActorMaxHP() const;
	int  ActorShield() const;

	void ActorAddShield(int v);
	int  ActorTakeDamage(int dmg); // TakeDamage 호출
	void ActorAddDot(int dmg, int ticks);


	void ActorSetReflect(float pct, int hits);
	float ActorGetReflectPct() const;
	int   ActorGetReflectHitsLeft() const;
	void  ActorConsumeReflectHit();

	// 행도 카드 확인용
	// ===== CardRule 적용용: 마지막으로 사용한 방어 카드 정보(상성/랭크 보정) =====
	bool m_hasLastDefCard = false;
	CAttribute m_lastDefAttr = E_BREAD;
	Star m_lastDefRank = E_ONE;

	int m_PlayLimitThisTurn = 1;   // 이번 턴 사용 가능 횟수
	int m_PlaysUsedThisTurn = 0;   // 이번 턴 사용한 횟수

	void RefreshPlayLimitFromPlayer();
	void TickDotsAtTurnEnd(CardManager& cur, CardManager& opp);
	void TryEndTurn(CardManager& opponent, HWND hWnd);

	void OpponentAct(CardManager& player);
};