#include "Shop.h"

//상자를 선택함
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 6; i++)
	{
		if (InCircle(chest[i].x, chest[i].y, p_mx, p_my))
		{
			this->isSelect = true;
			this->selectedChest = chest[i];	//좌표값에 따라 상자선택하기
			return;
		}
	}
}

void Shop::DrawShop(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
{
	//상자 정보 하이라이트 출력용
	Rectangle(p_hdc, 50, 50, 1000, 600);
	if (isSelect)
	{
		for (int i = 0; i < 4; i++)
		{
			wsprintf(p_text, TEXT("%d성 - %d%%"), i + 1, selectedChest.GetProb(i));
			if (i == 3)
			{
				wsprintf(p_text, TEXT("마법 - %d%%"), selectedChest.GetProb(i));
			}
			TextOut(p_hdc, 100 + i * 100, 500, p_text, lstrlen(p_text));
		}
		wsprintf(p_text, TEXT("%d"), selectedChest.x / 200);
		TextOut(p_hdc, 525, 300, p_text, lstrlen(p_text));
	}

	//상자들 출력
	for (int i = 0; i < 6; i++)
	{
		Rectangle(p_hdc, chest[i].x - 25, chest[i].y - 25, chest[i].x + 25, chest[i].y + 25);
	}
	wsprintf(p_text, TEXT("상점"));
	TextOut(p_hdc, 200, 25, p_text, lstrlen(p_text));
}