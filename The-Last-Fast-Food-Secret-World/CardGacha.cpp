#include "CardGacha.h"

CardGacha g_Gacha;

void CardGacha::one(DeckBuilding& p_deck, MainGame& p_mg)
{
	int invensize = p_deck.GetSize();//카드풀에서 랜덤한 카드를 뽑음
	draw_card.resize(1);
	int randIdx = rand() % AllCARDMAXSIZE;
	int ID = allCard[randIdx]->GetUid();
	draw_card[0].SetUid(ID);
	Star STAR = allCard[randIdx]->GetStar();
	draw_card[0].SetStar(STAR);
	CType TYPE = allCard[randIdx]->GetType();
	draw_card[0].SetType(TYPE);

	//카드가 추가될때 중복카드는 뽑기결과에서 나오되, 인벤에서 제외되어야함.
	if (isObtain[randIdx])
	{
		//중복 시 임시로 아이디를 음수로 변경 (기본카드의 경우는 예외를 설정할 예정)
		cout << draw_card[0].GetUid() << "는 중복" << "\n";
		draw_card[0].SetUid(-1);
		p_mg.AddGold(100 * 4); //고정값 * 레어도별 가중치 페이백
	}
	else
	{
		//신규 획득 카드는 얻었다는 표시를 설정
		cout << draw_card[0].GetUid() << "는 신규" << "\n";
		isObtain[randIdx] = TRUE;
	}
	int x = invensize % 5, y = invensize / 5;
	draw_card[0].x = x * 75 + 1050, draw_card[0].y = y * 150 + 50;
	p_deck.PushCard(draw_card);
}

void CardGacha::ten(DeckBuilding& p_deck, MainGame& p_mg)
{
	int invensize = p_deck.GetSize();
	draw_card.resize(10);
	for (int i = 0; i < 10; i++)
	{
		//카드풀에서 랜덤한 카드를 뽑음
		int randIdx = rand() % AllCARDMAXSIZE;
		int ID = allCard[randIdx]->GetUid();
		draw_card[i].SetUid(ID);
		Star STAR = allCard[randIdx]->GetStar();
		draw_card[i].SetStar(STAR);
		CType TYPE = allCard[randIdx]->GetType();
		draw_card[i].SetType(TYPE);

		//카드가 추가될때 중복카드는 뽑기결과에서 나오되, 인벤에서 제외되어야함.
		if (isObtain[randIdx])
		{
			//중복 시 임시로 아이디를 음수로 변경 (기본카드의 경우는 예외를 설정할 예정)
			cout << draw_card[i].GetUid() << "는 중복" << "\n";
			draw_card[i].SetUid(-1);
			p_mg.AddGold(100 * 4); //고정값 * 레어도별 가중치 페이백
		}
		else
		{
			//신규 획득 카드는 얻었다는 표시를 설정
			cout << draw_card[i].GetUid() << "는 신규" << "\n";
			isObtain[randIdx] = TRUE;
		}
	}
	p_deck.PushCard(draw_card);
}

//뽑기를 함(TRUE-1뽑 / FALSE-10뽑, 뽑은카드를 저장할 변수)
void CardGacha::GetGacha(bool isOne, DeckBuilding& p_deck, MainGame& p_mg, Chest p_selChest)
{
	isOneGacha = isOne;
	//분기나눠서 1차 or 10차 나누기
	if (isOneGacha) one(p_deck, p_mg);
	else if (!isOneGacha) ten(p_deck, p_mg);
}

void CardGacha::InGacha()
{
	//뽑기의 종류에 따라 출력방식 변경
	if (isOneGacha)
	{
		draw_card[0].x = 700, draw_card[0].y = 350;
	}
	else
	{
		for (int i = 0; i < 10; i++)
		{
			draw_card[i].x = (i % 5) * 250 + 200, draw_card[i].y = (i / 5) * 250 + 200;
		}
	}
}

void CardGacha::EnterGacha()
{
	RENDER.SetImage(L"rect_button.png", "back", Rect(0, 0, 100, 110)
		, Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
	//돌아가는 버튼위치 고민중
	m_GachaBtnM.AddButton(make_shared<RectButton>("back", RECT{ 50, 600, 250, 660 }));
}

void CardGacha::DrawGacha(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[])
{
	//뽑은카드 출력하게 수정할것///////////////////
	for (int i = 0; i < draw_card.size(); i++)
	{
		RENDER.MoveImage(to_string(draw_card[i].GetUid()), Rect(draw_card[i].x - 120, draw_card[i].y - 66, 100, 132));
		RENDER.ImageVisible(to_string(draw_card[i].GetUid()), true);
	}
	RENDER.ImageVisible("back", true);
	m_GachaBtnM.DrawAll();
}

void CardGacha::ExitGacha()
{
	for (int i = 0; i < draw_card.size(); i++)
	{
		RENDER.ImageVisible(to_string(draw_card[i].GetUid()), false);
	}
	cout << "버튼 삭제" << endl;
	RENDER.RemoveIDIamage("back");
}
