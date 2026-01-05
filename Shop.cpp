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

void Shop::CancelSeletion()
{
	this->isSelect = FALSE;
}

void Shop::DrawShop(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//상자 정보 하이라이트 출력용
	RoundRect(p_hdc, 50, 500, 550, 700, 100, 100);
	if (this->isSelect)
	{
		for (int i = 0; i < 4; i++)
		{
			wsprintf(p_text, TEXT("%d성 - %d%%"), i + 1, selectedChest.GetProb(i));
			if (i == 3)
			{
				wsprintf(p_text, TEXT("마법 - %d%%"), selectedChest.GetProb(i));
			}
			TextOut(p_hdc, 100 + i * 100,  675, p_text, lstrlen(p_text));
		}
		wsprintf(p_text, TEXT("%d"), selectedChest.GetChestID());
		TextOut(p_hdc, 100, 625, p_text, lstrlen(p_text));
	}

	for (int i = 0; i < 2; i++)
	{
		int startx = 700, starty = 300, gap = i * 200;
		POINT tjsqks[12] = { {startx, starty + gap}, {startx + 500, starty + gap}, {startx + 500, starty + 50 + gap}
			, {startx, starty + 50 + gap}, {startx, starty + gap}, {startx + 150, starty - 50 + gap}
			, {startx + 650, starty - 50 + gap}, {startx + 500, starty + gap}, {startx + 500, starty + 50 + gap}
		, {startx + 650, starty + gap}, {startx + 650, starty - 50 + gap}, {startx + 500, starty + gap} };
		Polygon(p_hdc, tjsqks, 12);
	}

	//상자들 출력
	for (int i = 0; i < 6; i++)
	{
		Rectangle(p_hdc, chest[i].x - 25, chest[i].y - 25, chest[i].x + 25, chest[i].y + 25);
		wsprintf(p_text, TEXT("%d"), chest[i].GetChestID());
		TextOut(p_hdc, chest[i].x, chest[i].y, p_text, lstrlen(p_text));
	}
	wsprintf(p_text, TEXT("상점"));
	TextOut(p_hdc, 200, 25, p_text, lstrlen(p_text));
}
