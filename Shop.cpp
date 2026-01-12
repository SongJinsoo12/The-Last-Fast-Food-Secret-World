#include "Shop.h"

//상자를 선택함
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 6; i++)
	{
		if (InCircle(chest[i].x, chest[i].y, p_mx, p_my))
		{
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
	this->isSelect = FALSE;
}

void Shop::DrawShop(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//상자 보관용 선반
	renderManager_shop->SetImage(L"shelf.png", "shelf", Rect(0, 0, 651, 101), Rect(700, 250, 651, 101));
	renderManager_shop->SetImage(L"shelf.png", "shelf2", Rect(0, 0, 651, 101), Rect(700, 450, 651, 101));

	//상점 주인
	renderManager_shop->SetImage(L"cookie.png", "cookie", Rect(0, 0, 2500, 2500), Rect(300 - 180, 430 - 180, 360, 360));

	//상자 정보 하이라이트 및 상점 주인 대사 출력용
	renderManager_shop->SetImage(L"textbox.png", "textbox", Rect(0, 0, 500, 200), Rect(50, 500, 500, 200));

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

	//상자들 출력
	for (int i = 0; i < 6; i++)
	{
		renderManager_shop->SetImage(L"chest.png", "chest" + to_string(i), Rect(0, 0, 1024, 1024), Rect(chest[i].x - 48, chest[i].y - 78, 128, 128));
		//Rectangle(p_hdc, chest[i].x - 25, chest[i].y - 25, chest[i].x + 25, chest[i].y + 25);
		wsprintf(p_text, TEXT("%d"), chest[i].GetChestID());
		TextOut(p_hdc, chest[i].x, chest[i].y, p_text, lstrlen(p_text));
	}
	wsprintf(p_text, TEXT("상점"));
	TextOut(p_hdc, 200, 25, p_text, lstrlen(p_text));
}
