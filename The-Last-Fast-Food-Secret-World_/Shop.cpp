#include "Shop.h"

Shop g_Shop;

bool Shop::isSucceedGacha(HDC p_hdc, int p_gacha_num)
{
	int remove_gold = selectedChest.GetPrice() * p_gacha_num;
	if (g_MainGame.RemoveGold(remove_gold))
	{
		return true;
	}
	else
	{
		TextOut(p_hdc, 50, 500, TEXT("돈이 부족합니다."), 10);
		return false;
	}
}

void Shop::DrawGachaButton(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[])
{
	//버튼생성위치 옮기고 여기서는 온오프만 관리
	//1뽑 버튼
	m_ShopBtnM.AddButton(make_shared<RectButton>("one", RECT{ 700, 615, 1020, 685 }));
	wsprintf(p_text, TEXT("1개 - %dG"), selectedChest.GetPrice());
	TextOut(p_hdc, 830, 645, p_text, lstrlen(p_text));

	//10뽑 버튼
	m_ShopBtnM.AddButton(make_shared<RectButton>("ten", RECT{ 1050, 615, 1370, 685 }));
	wsprintf(p_text, TEXT("10개 - %dG"), selectedChest.GetPrice() * 9);
	TextOut(p_hdc, 1170, 645, p_text, lstrlen(p_text));
}

//상자를 선택함
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_ShopBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "chest" + to_string(i))
		{
			RENDER.ImageVisible("one", true);
			RENDER.ImageVisible("ten", true);
			this->isSelect = TRUE;
			this->selectedChest = chest[i];	//좌표값에 따라 상자선택하기
			printf("%d번 상자 선택\n", selectedChest.GetChestID());
			return;
		}
	}
}

BOOL Shop::CheckIsSelection()
{
	if (this->isSelect) return TRUE;
	else return FALSE;
}

void Shop::CancelSelection()
{
	RENDER.ImageVisible("one", false);
	RENDER.ImageVisible("ten", false);
	this->isSelect = FALSE;
}

void Shop::SetDrawShop()
{
	RENDER.ImageVisible("shelf1", true);//상자 보관용 선반1
	RENDER.ImageVisible("shelf2", true);//상자 보관용 선반2

	//상자 정보 및 상점 주인 대사 출력용
	RENDER.ImageVisible("textbox", true);
}

void Shop::SetEnterShop()
{
	mov_sel = 1400;
	ani_start = chrono::steady_clock::now();
}

void Shop::ExitShop()
{
	RENDER.ImageVisible("shelf1", false);
	RENDER.ImageVisible("shelf2", false);
	RENDER.ImageVisible("cookie", false);
	RENDER.ImageVisible("textbox", false);
	for (int i = 0; i < 6; i++) RENDER.ImageVisible("chest" + to_string(i), false);
	RENDER.ImageVisible("one", false);
	RENDER.ImageVisible("ten", false);
	mov_sel = 1400;
	this->CancelSelection();
}

void Shop::DrawShop(HDC p_hdc, WCHAR p_text[])
{
	if (this->isSelect)
	{
		for (int i = 0; i < 4; i++)
		{
			wsprintf(p_text, TEXT("%d성 - %d%%"), i + 1, selectedChest.GetProb(i));
			if (i == 3)
			{
				wsprintf(p_text, TEXT("마법 - %d%%"), selectedChest.GetProb(i));
			}
			TextOut(p_hdc, 100 + i * 100, 675, p_text, lstrlen(p_text));
		}
		wsprintf(p_text, TEXT("%d"), selectedChest.GetChestID());
		TextOut(p_hdc, 100, 625, p_text, lstrlen(p_text));
	}

	m_ShopBtnM.DrawAll();
}

bool Shop::DrawEnterShop()
{
	if (mov_sel <= 710) return true;
	//MoveImageTween("shelf1", Rect(), 3).OnComplete( EndFN );
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - ani_start).count();

	RENDER.MoveImage("shelf1", Rect(mov_sel - 10, 250, 651, 101));
	RENDER.MoveImage("shelf2", Rect(mov_sel - 10, 450, 651, 101));
	RENDER.MoveImage("textbox", Rect((700 - mov_sel) + 50 + 10, 500, 500, 200));

	int dur = 10;

	if (elapsed < dur)
	{
		mov_sel -= (int)((mov_sel - 700) / 10);
		if (mov_sel <= 710)//입장 애니메이션이 끝난 후 나머지 이미지 활성화
		{
			for (int i = 0; i < 6; i++)
			{
				m_ShopBtnM.AddButton(make_shared<CircleButton>("chest" + to_string(i), chest[i].x + 10, chest[i].y - 5, 64));
				RENDER.ImageVisible("chest" + to_string(i), true);//상자 출력
			}
			RENDER.ImageVisible("cookie", true);// 상점 주인
			return true;
		}
	}
	else
	{
		ani_start = std::chrono::steady_clock::now();
		return false;
	}
}