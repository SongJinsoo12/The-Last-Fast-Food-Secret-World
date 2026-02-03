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