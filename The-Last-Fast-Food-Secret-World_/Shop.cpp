#include "Shop.h"

Shop g_Shop;

void Shop::DrawGachaButton(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[])
{
	//버튼생성위치 옮기고 여기서는 온오프만 관리
	//1뽑 버튼
	btnManager.AddButton(make_shared<RectButton>("one", RECT{ 700, 615, 1020, 685 }));
	wsprintf(p_text, TEXT("1개 - %dG"), selectedChest.GetPrice());
	TextOut(p_hdc, 830, 645, p_text, lstrlen(p_text));

	//10뽑 버튼
	btnManager.AddButton(make_shared<RectButton>("ten", RECT{ 1050, 615, 1370, 685 }));
	wsprintf(p_text, TEXT("10개 - %dG"), selectedChest.GetPrice() * 9);
	TextOut(p_hdc, 1170, 645, p_text, lstrlen(p_text));
}

//상자를 선택함
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 3; i++)
	{
		if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "chest" + to_string(i))
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
}