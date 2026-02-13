#include "CardManager.h"
#include "Card.h"
#include "RenderManager.h"
#include "ImageLoad.h"
#include "ImageManager.h"
#include "CardTableManager.h"
#include "Boss.h"
#include "Player.h"
#include "InputGame.h"
#include "Mob.h"
#include "DefCard.h"
#include "AtkCard.h"
#include "SupportCard.h"
#include "CardRule.h"

CardManager::CardManager() : m_DeckCount(25), m_HandCount(0), m_HandSelection(4),
m_IsMyTurn(false), m_IsSelect(false), m_isShiny(false), m_isRip(false)
{
}

void CardManager::SetImage()
{
	RENDER.SetImage(L"background_city_night.png", "City_Night",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	RENDER.SetImage(L"background_pink_plant.png", "Plant_Pink",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	RENDER.SetImage(L"background_orange_plant.png", "Plant_Orange",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);
	RENDER.SetImage(L"background_dark_plant.png", "Plant_Dark",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), true,
		GameImage_M::LayerType::Background);
	RENDER.SetImage(L"background_ari_ship.png", "Air_Ship",
		Gdiplus::Rect(0, 0, 2304, 1296), Gdiplus::Rect(0, 0, 1280, 720), false,
		GameImage_M::LayerType::Background);

	RENDER.SetImage(L"card_zone.png", "Card_Middle_Up",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	RENDER.SetImage(L"card_zone.png", "Card_Middle_Down",
		Gdiplus::Rect(0, 0, 88, 110), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	RENDER.SetImage(L"card_back_2.png", "Card_Deck_Up",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);
	RENDER.SetImage(L"card_back.png", "Card_Deck_Down",
		Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
		GameImage_M::LayerType::Background);

	for (size_t i = 0; i < 30; i++)
	{
		string cardId = "Card_Boss_Hand_";
		cardId = cardId + to_string(i);

		RENDER.SetImage(L"card_back_2.png", cardId,
			Gdiplus::Rect(0, 0, CARDX, CARDY), Gdiplus::Rect(0, 0, 0, 0), true,
			GameImage_M::LayerType::Background);
	}
	for (size_t i = 0; i < 10; i++)
	{
		string cardPng = "card_shiny_";
		string cardId = "Card_Shiny_";
		cardPng += to_string(i + 1);
		cardPng += ".png";
		cardId += to_string(i);

		wstring wCardPng;
		wCardPng.assign(cardPng.begin(), cardPng.end());

		RENDER.SetImage(wCardPng, cardId,
			Gdiplus::Rect(0, 0, 100, 128), Gdiplus::Rect(0, 0, 0, 0), false,
			GameImage_M::LayerType::UI);
	}
	for (size_t i = 0; i < 15; i++)
	{
		if (i < 12)
		{
			string cardPng = "card_rip_";
			string cardId = "Card_Rip_";
			cardPng += to_string(i + 1);
			cardPng += ".png";
			cardId += to_string(i);

			wstring wCardPng;
			wCardPng.assign(cardPng.begin(), cardPng.end());

			RENDER.SetImage(wCardPng, cardId,
				Gdiplus::Rect(0, 0, 100, 128), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::UI);
		}
		else if (i == 12)
		{
			string cardPng = "card_rip_";
			string cardId = "Card_Rip_";
			cardPng += to_string(i + 1);
			cardPng += ".png";
			cardId += to_string(i);

			wstring wCardPng;
			wCardPng.assign(cardPng.begin(), cardPng.end());

			RENDER.SetImage(wCardPng, cardId,
				Gdiplus::Rect(0, 0, 108, 155), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::UI);
		}
		else if (i == 13)
		{
			string cardPng = "card_rip_";
			string cardId = "Card_Rip_";
			cardPng += to_string(i + 1);
			cardPng += ".png";
			cardId += to_string(i);

			wstring wCardPng;
			wCardPng.assign(cardPng.begin(), cardPng.end());

			RENDER.SetImage(wCardPng, cardId,
				Gdiplus::Rect(0, 0, 113, 177), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::UI);
		}
		else if (i == 14)
		{
			string cardPng = "card_rip_";
			string cardId = "Card_Rip_";
			cardPng += to_string(i + 1);
			cardPng += ".png";
			cardId += to_string(i);

			wstring wCardPng;
			wCardPng.assign(cardPng.begin(), cardPng.end());

			RENDER.SetImage(wCardPng, cardId,
				Gdiplus::Rect(0, 0, 122, 182), Gdiplus::Rect(0, 0, 0, 0), false,
				GameImage_M::LayerType::UI);
		}
	}

	cout << "이미지 로드 확인\n";
}

//카드 이펙트 애니메이션
void CardManager::PlayCardEffect(int x, int y)
{
	if (!m_isShiny) return;

	if (!m_shiny.GetIsStart())
	{
		m_shiny.StartTimer();
		m_shiny.SetIsStart(true);
	}

	string cardId = "Card_Shiny_";
	cardId += to_string(m_shiny.GetIndex());
	RENDER.MoveImage(cardId, Gdiplus::Rect(x, y, 100, 128));
	RENDER.ImageVisible(cardId, true);

	m_shiny.UpdateTimer();
	if (m_shiny.CheckTimer(0.05))
	{
		RENDER.ImageVisible(cardId, false);

		//이펙트 끝
		if (m_shiny.GetIndex() >= 10)
		{
			m_shiny.SetIndex(0);
			m_isShiny = false;
			return;
		}

		m_shiny.StartTimer();
		m_shiny.PlusIndex();
		string newId = "Card_Shiny_";
		newId += to_string(m_shiny.GetIndex());
		RENDER.MoveImage(newId, Gdiplus::Rect(x, y, 100, 128));
		RENDER.ImageVisible(newId, true);
	}
}

void CardManager::PlayRip(int x, int y)
{
	if (!m_isRip) return;

	if (!m_rip.GetIsStart())
	{
		m_rip.StartTimer();
		m_rip.SetIsStart(true);
	}
	string cardId = "Card_Rip_";
	cardId += to_string(m_rip.GetIndex());
	if (m_rip.GetIndex() < 12)
	{
		RENDER.MoveImage(cardId, Gdiplus::Rect(x, y, 108, 128));
		RENDER.ImageVisible(cardId, true);
	}
	else if (m_rip.GetIndex() == 12)
	{
		RENDER.MoveImage(cardId, Gdiplus::Rect(x, y, 108, 155));
		RENDER.ImageVisible(cardId, true);
	}
	else if (m_rip.GetIndex() == 13)
	{
		RENDER.MoveImage(cardId, Gdiplus::Rect(x, y, 113, 177));
		RENDER.ImageVisible(cardId, true);
	}
	else if (m_rip.GetIndex() == 14)
	{
		RENDER.MoveImage(cardId, Gdiplus::Rect(x, y, 122, 182));
		RENDER.ImageVisible(cardId, true);
	}

	m_rip.UpdateTimer();
	if (m_rip.CheckTimer(0.03))
	{
		RENDER.ImageVisible(cardId, false);

		//이펙트 끝
		if (m_rip.GetIndex() >= 15)
		{
			m_rip.SetIndex(0);
			m_isRip = false;
			return;
		}

		m_rip.StartTimer();
		m_rip.PlusIndex();
		string newId = "Card_Rip_";
		newId += to_string(m_rip.GetIndex());
		if (m_rip.GetIndex() < 12)
		{
			RENDER.MoveImage(newId, Gdiplus::Rect(x, y, 108, 128));
			RENDER.ImageVisible(newId, true);
		}
		else if (m_rip.GetIndex() == 12)
		{
			RENDER.MoveImage(newId, Gdiplus::Rect(x, y, 108, 155));
			RENDER.ImageVisible(newId, true);
		}
		else if (m_rip.GetIndex() == 13)
		{
			RENDER.MoveImage(newId, Gdiplus::Rect(x, y, 113, 177));
			RENDER.ImageVisible(newId, true);
		}
		else if (m_rip.GetIndex() == 14)
		{
			RENDER.MoveImage(newId, Gdiplus::Rect(x, y, 122, 182));
			RENDER.ImageVisible(newId, true);
		}
	}
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
}

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

	if (uid == 143)
	{
		card->SetAtk(0);
		card->SetDef(0);
		card->SetAit(E_BREAD);
		card->SetType(E_Deffense);
		card->SetStar(E_TWO);
		return;
	}

	// 보조 카드 : 200~233 (프로젝트 UID 범위)
	if (201 <= uid && uid <= 232)
	{
		card->SetAtk(0);
		card->SetDef(0);
		card->SetAit(E_BREAD);
		card->SetType(E_Magic);
		card->SetStar(E_TWO);
		return;
	}
}

void CardManager::ApplyAttackTo(CardManager& opponent, GameCard* card)
{
	if (!card) return;

	const int uid = card->GetUid();
	const CAttribute attr = card->GetAit();
	const Star rank = card->GetStar();

	AtkCard atkRule;

	// 1) 룰로 데미지 계산
	int dmg = 0;

	if (1 <= uid && uid <= 36)
	{
		dmg = atkRule.DefaultAtk(attr, rank);
	}
	else if (uid == 37)
	{
		bool iAmDamaged = (ActorHP() < ActorMaxHP());
		if (iAmDamaged)
		{
			switch (rank)
			{
			case E_ONE:   dmg = 2; break;
			case E_TWO:   dmg = 3; break;
			case E_THREE: dmg = 4; break;
			default:      dmg = 2; break;
			}
		}
		else dmg = 0;
	}
	else if (uid == 38)
	{
		dmg = atkRule.coinAtk(attr, rank);
	}
	else if (uid == 39)
	{
		bool defUsed = (opponent.ActorShield() > 0); // 상대가 실드가 있으면 방어 사용으로 간주(최소 구현)
		dmg = atkRule.DefCard_After_Atk(defUsed, rank);
	}
	else if (uid == 40 && attr == E_BREAD)
	{
		int base = atkRule.PoisonDamageCard(rank); // 2/3/4
		int dotDmg = base - 1;                     // 1/2/3로 변환
		if (dotDmg < 1) dotDmg = 1;

		const int dotTicks = 6;
		opponent.ActorAddDot(dotDmg, dotTicks);

		std::cout << "[BREAD_ATK] uid40 DOT 부여: dmg=" << dotDmg << ", ticks=" << dotTicks << "\n";
		dmg = 0;
	}
	else
	{
		dmg = card->GetAtk(); // 안전장치
	}

	if (dmg < 0) dmg = 0;

	// ===== CardRule: 상대가 마지막으로 쓴 방어 카드가 있으면 상성/랭크 보정 =====
	if (opponent.m_hasLastDefCard && opponent.ActorShield() > 0)
	{
		int bonus = 0;
		bonus += CardRule::RankMatchUp(rank, opponent.m_lastDefRank);
		if (CardRule::IsCounterAttribute(attr, opponent.m_lastDefAttr))
			bonus += 1;
		if (bonus != 0)
		{
			std::cout << "[CardRule] bonus=" << bonus << " (atkRank=" << rank << ", defRank=" << opponent.m_lastDefRank << ")\n";
			dmg += bonus;
		}
	}

	// 2) 실제 HP/Shield 적용은 Actor(Player/Mob)에게 위임 (추천안 핵심)
	int beforeHP = opponent.ActorHP();
	int beforeShield = opponent.ActorShield();

	opponent.ActorTakeDamage(dmg);

	// ===== 피해 반사(DEF 143): 다음 공격 1회 30% 반사 =====
	if (dmg > 0 && opponent.ActorGetReflectPct() > 0.0f && opponent.ActorGetReflectHitsLeft() > 0)
	{
		int ref = (int)(dmg * opponent.ActorGetReflectPct());
		if (ref < 1) ref = 1;
		int selfHP0 = ActorHP();
		ActorTakeDamage(ref);
		opponent.ActorConsumeReflectHit();
		std::cout << "[전투] 반사 데미지=" << ref << " | self HP " << selfHP0 << "->" << ActorHP() << "\n";
	}

	std::cout << "[전투] 공격 UID=" << uid
		<< " dmg=" << dmg
		<< " | target HP " << beforeHP << "->" << opponent.ActorHP()
		<< " | target Shield " << beforeShield << "->" << opponent.ActorShield()
		<< "\n";
}

void CardManager::ApplyDefense(GameCard* card)
{
	if (!card) return;

	// CardRule용: 마지막으로 사용한 방어 카드 기록
	m_hasLastDefCard = true;
	m_lastDefAttr = card->GetAit();
	m_lastDefRank = card->GetStar();
	const int uid = card->GetUid();
	const CAttribute attr = card->GetAit();
	const Star rank = card->GetStar();

	DefCard defRule;

	int add = 0;

	if (101 <= uid && uid <= 120)
		add = defRule.DefaultDef(attr, rank);
	else if (uid == 141)
		add = defRule.BreadDef(attr, rank);
	else if (uid == 142)
		add = 999;
	else if (uid == 143)
		add = 0;
	else
		add = card->GetDef();

	if (add < 0) add = 0;

	if (uid == 143)
	{
		ActorSetReflect(0.30f, 1);
		std::cout << "[전투] 반사 버프(30%) 부여 (DEF uid=143)\n";
	}

	ActorAddShield(add);

	std::cout << "[전투] 방어 UID=" << uid
		<< " +" << add
		<< " -> 내 HP " << ActorHP()
		<< ", 내 방어막 " << ActorShield() << "\n";
}

void CardManager::ApplySupport(GameCard* card, CardManager& opponent)
{
	if (!card) return;

	const int uid = card->GetUid();

	// 바인딩이 안 되어 있으면 (Player/Mob 포인터 주입 실패) 최소 로그만 남기고 스킵
	if (!m_pPlayer || !m_pEnemyMob)
	{
		std::cout << "[SUP] uid=" << uid << " skipped (unbound)";
		return;
	}

	// 실행 전/후 상태만 간단히 출력 (HP/Shield 변화 확인용)
	const int myHP0 = ActorHP();
	const int mySH0 = ActorShield();
	const int opHP0 = opponent.ActorHP();
	const int opSH0 = opponent.ActorShield();

	SupportCard sc;
	sc.ApplyByUid(uid, *m_pPlayer, *m_pEnemyMob, *this);

	const int myHP1 = ActorHP();
	const int mySH1 = ActorShield();
	const int opHP1 = opponent.ActorHP();
	const int opSH1 = opponent.ActorShield();

	std::cout << "[SUP] uid=" << uid
		<< " | my(H:" << myHP0 << "->" << myHP1 << " S:" << mySH0 << "->" << mySH1 << ")"
		<< " | opp(H:" << opHP0 << "->" << opHP1 << " S:" << opSH0 << "->" << opSH1 << ")";
}

int CardManager::ActorHP() const
{
	if (m_pPlayer)
	{
		return m_pPlayer->GetHP();
	}
	if (m_pEnemyMob)
	{
		return m_pEnemyMob->GetHP();
	}
	return 0;
}

int CardManager::ActorMaxHP() const
{
	if (m_pPlayer) return m_pPlayer->GetMaxHP();
	if (m_pEnemyMob) return m_pEnemyMob->GetMaxHP();
	return 0;
}

int CardManager::ActorShield() const
{
	if (m_pPlayer) return m_pPlayer->GetShield();
	if (m_pEnemyMob) return m_pEnemyMob->GetShield();
	return 0;
}

void CardManager::ActorAddShield(int v)
{
	if (m_pPlayer) m_pPlayer->AddShield(v);
	else if (m_pEnemyMob) m_pEnemyMob->AddShield(v);
}

int CardManager::ActorTakeDamage(int dmg)
{
	if (m_pPlayer) return m_pPlayer->TakeDamage(dmg);
	if (m_pEnemyMob) return m_pEnemyMob->TakeDamage(dmg);
	return 0;
}

void CardManager::ActorAddDot(int dmg, int ticks)
{
	if (m_pPlayer) m_pPlayer->AddDot(dmg, ticks);
	else if (m_pEnemyMob) m_pEnemyMob->AddDot(dmg, ticks);
}

void CardManager::ActorSetReflect(float pct, int hits)
{
	if (m_pPlayer) m_pPlayer->SetReflect(pct, hits);
	else if (m_pEnemyMob) m_pEnemyMob->SetReflect(pct, hits);
}

float CardManager::ActorGetReflectPct() const
{
	if (m_pPlayer) return m_pPlayer->GetReflectPct();
	if (m_pEnemyMob) return m_pEnemyMob->GetReflectPct();
	return 0.0f;
}

int CardManager::ActorGetReflectHitsLeft() const
{
	if (m_pPlayer) return m_pPlayer->GetReflectHitsLeft();
	if (m_pEnemyMob) return m_pEnemyMob->GetReflectHitsLeft();
	return 0;
}

void CardManager::ActorConsumeReflectHit()
{
	if (m_pPlayer) m_pPlayer->SetReflect(0.0f, 0);
	else if (m_pEnemyMob) m_pEnemyMob->SetReflect(0.0f, 0);
}

void CardManager::RefreshPlayLimitFromPlayer()
{
	// Player가 바인딩되어 있으면 Player의 플레이 제한을 따라감
	if (m_pPlayer)
		m_PlayLimitThisTurn = m_pPlayer->GetPlayLimitThisTurn();
	else
		m_PlayLimitThisTurn = 1;
}

void CardManager::TickDotsAtTurnEnd(CardManager& cur, CardManager& opp)
{
	// cur, opp 각각이 바인딩한 Actor에게 DOT 적용
	if (cur.m_pPlayer) cur.m_pPlayer->TickDots();
	else if (cur.m_pEnemyMob) cur.m_pEnemyMob->TickDots();

	if (opp.m_pPlayer) opp.m_pPlayer->TickDots();
	else if (opp.m_pEnemyMob) opp.m_pEnemyMob->TickDots();
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

	// 여기부터가 “진짜 턴 종료”
	TickDotsAtTurnEnd(*this, opponent);

	this->m_IsMyTurn = !this->m_IsMyTurn;
	opponent.m_IsMyTurn = !opponent.m_IsMyTurn;

	// ✅ 상대 턴 타이머를 새로 시작하게 초기화
	opponent.m_timer.SetIsStart(false);

	// 상대 행동은 “즉시”가 아니라 타이머로 처리하고 싶으면 OpponentAct를 여기서 빼도 됨
	// 일단 기존 구조 유지 시:
// 	std::cout << "상대방의 턴\n";
	opponent.OpponentAct(*this);
}

void CardManager::OpponentAct(CardManager& player)
{
	BossCardAct(player);
}

void CardManager::StartGame()
{
	CardDraw(5);
	// 	cout << "처음 카드 드로우 확인\n";
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
		RENDER.ImageVisible(to_string(m_Deck[m_DeckCount]->GetUid()), true);
	}

	//패 카드 임시 확인
// 	cout << "패 카드 번호: [ ";
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
	RENDER.MoveImage("Card_Middle_Up",
		Gdiplus::Rect(midX - cardMidX, midY - (deckY + 10), deckX, deckY));
	RENDER.MoveImage("Card_Middle_Down",
		Gdiplus::Rect(midX - cardMidX, (midY + 10), deckX, deckY));

	RENDER.MoveImage("Card_Deck_Up",
		Gdiplus::Rect(0, 0, deckX, deckY));
	RENDER.MoveImage("Card_Deck_Down",
		Gdiplus::Rect(1265 - deckX, 682 - deckY, deckX, deckY));


	// 	cout << "배경 출력 확인\n";
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
				RENDER.MoveImage(to_string(m_Hand[m_HandSelection]->GetUid()),
					Gdiplus::Rect(70, 380, CARDX * 2, CARDY * 2));
			}
			else
			{
				RENDER.MoveImage(to_string(m_Hand[i]->GetUid()),
					Gdiplus::Rect(startPos, posY - 10, CARDX, CARDY));
			}
		}
		else
		{
			RENDER.MoveImage(to_string(m_Hand[i]->GetUid()),
				Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
		}

		//카드 출력 순서
		RENDER.LayerMoveToBack(to_string(m_Hand[i]->GetUid()));
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
		RENDER.MoveImage(cardId,
			Gdiplus::Rect(startPos, posY + 10, CARDX, CARDY));
	}
}

//패 카드 사용
void CardManager::CardAct(CardManager& player, CardManager& opponent)
{
	if (!m_IsMyTurn) return; // ✅ 내 턴 아니면 카드 사용 금지
	//패에 카드가 없으면 리턴
	if (m_HandCount <= 0)
		return;
	//선택 중이지 않으면 리턴
	if (m_HandSelection < 0)
		return;

	//타이머 초기화
	m_timer.SetIsStart(false);

	// =========================================================
	// 디버그: 카드 실행 흐름을 콘솔에 자세히 출력
	//  - UID/타입/속성/별/ATK/DEF
	//  - 어떤 Apply* 함수로 들어갔는지
	//  - HP/Shield, 패/덱 변화(핵심만)
	// =========================================================
	GameCard* sel = m_Hand[m_HandSelection];
	if (!sel) return;

	// 마지막 사용 카드 기록(UID 211 지원용)
	if (player.m_pPlayer) player.m_pPlayer->setLastUsedCard(sel);
	else if (player.m_pEnemyMob) player.m_pEnemyMob->setLastUsedCard(sel);

	// 혹시 카드 데이터가 비어있더라도 UID 범위로 기본 매핑
	ApplyUidMapping(sel);

	auto TypeToStr = [](int t) -> const char*
		{
			switch (t)
			{
			case E_Attack:   return "Attack";
			case E_Deffense: return "Defense";
			case E_Magic:    return "Support";
			default:         return "Unknown";
			}
		};

	auto StarToStr = [](int s) -> const char*
		{
			switch (s)
			{
			case E_ONE:   return "1";
			case E_TWO:   return "2";
			case E_THREE: return "3";
			default:      return "?";
			}
		};

	auto AttrToStr = [](int a) -> const char*
		{
			switch (a)
			{
			case E_BULGOGI: return "BULGOGI";
			case E_SOURCE:  return "SOURCE";
			case E_CHESSE:  return "CHESSE";
			case E_VEGAT:   return "VEGAT";
			case E_BREAD:   return "BREAD";
			default:        return "ATTR?";
			}
		};

	std::cout
		<< "\n[CardAct] uid=" << sel->GetUid()
		<< " type=" << TypeToStr(sel->GetType())
		<< " attr=" << AttrToStr(sel->GetAit())
		<< " star=" << StarToStr(sel->GetStar())
		<< " atk=" << sel->GetAtk()
		<< " def=" << sel->GetDef()
		<< " | my HP=" << ActorHP() << "/" << ActorMaxHP()
		<< " shield=" << ActorShield()
		<< " | opp HP=" << opponent.ActorHP() << "/" << opponent.ActorMaxHP()
		<< " shield=" << opponent.ActorShield()
		<< "\n";

	switch (sel->GetType())
	{
	case E_Attack:
		// 		std::cout << "[Flow] ApplyAttackTo(opponent, card)\n";
		ApplyAttackTo(opponent, sel);
		// 		cout << "공격 카드 사용!! " << sel->GetAtk() << "데미지!!\n";
		break;
	case E_Deffense:
		// 		std::cout << "[Flow] ApplyDefense(card)\n";
		ApplyDefense(sel);
		// 		cout << "방어 카드 사용!!" << sel->GetDef() << "방어!!\n";
		break;
	case E_Magic:
		// 		std::cout << "[Flow] ApplySupport(card, opponent)\n";
		ApplySupport(sel, opponent);
		// 		cout << "보조 카드 사용!!\n";
		break;
	}

	std::cout
		<< "[After] my HP=" << ActorHP() << "/" << ActorMaxHP()
		<< " shield=" << ActorShield()
		<< " | opp HP=" << opponent.ActorHP() << "/" << opponent.ActorMaxHP()
		<< " shield=" << opponent.ActorShield()
		<< "\n";

	m_isShiny = true;
	m_isRip = true;

	//이미지 안보이기
	RENDER.ImageVisible(to_string(m_Hand[m_HandSelection]->GetUid()), false);
	/*int midX = 1280 * 0.5;
	int midY = 720 * 0.5;
	int cardMidX = CARDX * 0.5;
	int deckX = CARDX;
	int deckY = CARDY;
	M_REND.MoveImage(to_string(m_Hand[m_HandSelection]->GetUid()),
		Gdiplus::Rect(midX - cardMidX, (midY + 10), deckX, deckY));
	ShinyEffect(midX - cardMidX, (midY + 10));*/

	m_Hand.erase(m_Hand.begin() + m_HandSelection);
	m_HandCount--;
	//사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (m_HandSelection >= m_HandCount && m_HandSelection != 0)
		m_HandSelection--;

	// ===== 카드 사용 횟수 증가 후, 제한에 도달하면 즉시 보스 턴으로 넘김 =====
	RefreshPlayLimitFromPlayer();
	++m_PlaysUsedThisTurn;
	TryEndTurn(opponent, NULL);
}

void CardManager::HandSelect(CardManager& player, CardManager& opponent)
{
	if (GameInput_M::Input::GetInstance().isKeyboard((int)GameInput_M::KeyboardValue::ArrowLeft)
		&& !(m_HandSelection <= 0)) m_HandSelection--;
	else if (GameInput_M::Input::GetInstance().isKeyboard((int)GameInput_M::KeyboardValue::ArrowRight)
		&& !(m_HandSelection >= m_HandCount - 1)) m_HandSelection++;

	else if (GameInput_M::Input::GetInstance().isKeyboard((int)GameInput_M::KeyboardValue::ArrowUp)) m_IsSelect = true;
	else if (GameInput_M::Input::GetInstance().isKeyboard((int)GameInput_M::KeyboardValue::ArrowDown)) m_IsSelect = false;
	else if (GameInput_M::Input::GetInstance().isKeyboard((int)GameInput_M::KeyboardValue::Enter)
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
		//		cout << "자신의 턴\n";
	}
	else
	{
		opponent.m_IsMyTurn = !opponent.m_IsMyTurn;
		//		cout << "상대방의 턴\n";
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
	if (!m_timer.CheckTimer(5))
		return;

	// ✅ 여기서 먼저 끊어줘야 같은 프레임/다음 프레임 연속 진입을 막음
	m_timer.SetIsStart(false);

	// 1) 턴 토글
	player.m_IsMyTurn = !player.m_IsMyTurn;
	opponent.m_IsMyTurn = !opponent.m_IsMyTurn;

	// 2) 다음 턴 BeginTurn (실드 0은 여기서만)
	static int turnCounter = 1;
	++turnCounter;

	if (player.m_IsMyTurn)
	{
		player.CardDraw(1);
		if (player.m_pPlayer)        player.m_pPlayer->BeginTurn(turnCounter);
		else if (player.m_pEnemyMob) player.m_pEnemyMob->BeginTurn(turnCounter);
	}
	else
	{
		opponent.CardDraw(1);
		if (opponent.m_pPlayer)        opponent.m_pPlayer->BeginTurn(turnCounter);
		else if (opponent.m_pEnemyMob) opponent.m_pEnemyMob->BeginTurn(turnCounter);
		// opponent.BossCardAct(player); // 원할 때만
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
	// ✅ 보스 턴이 아닐 때는 행동 금지 (중복 실행/턴 꼬임 방지)
	if (!m_IsMyTurn) return;

	// 패에 카드가 없으면 리턴
	if (m_HandCount <= 0) return;

	// 선택 인덱스 보정 (보스는 선택 UI가 없어서 -1이면 0으로)
	if (m_HandSelection < 0) m_HandSelection = 0;
	if (m_HandSelection >= m_HandCount) m_HandSelection = 0;

	// (기존 효과 처리 로직이 따로 없어서, 지금은 '카드 1장 소비'만 유지)
	switch (m_Hand[m_HandSelection]->GetType())
	{
	case E_Attack:
		// 공격 로직은 ApplyAttack이 아니라면 별도 구현 필요
		break;
	case E_Deffense:
		break;
	case E_Magic:
		break;
	}

	// 이미지 안보이기
	RENDER.ImageVisible(to_string(m_Hand[m_HandSelection]->GetUid() + BOSSUID), false);

	m_Hand.erase(m_Hand.begin() + m_HandSelection);
	m_HandCount--;

	// 사용한 카드가 패의 가장 오른쪽 카드이면 왼쪽 카드 선택
	if (m_HandSelection >= m_HandCount && m_HandSelection != 0)
		m_HandSelection--;
}
