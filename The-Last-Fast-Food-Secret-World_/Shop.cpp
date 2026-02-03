<<<<<<< HEAD
ï»¿#include "Shop.h"

Shop g_Shop;

//ìƒìžë¥¼ ì„ íƒí•¨
=======
#include "Shop.h"

//»óÀÚ¸¦ ¼±ÅÃÇÔ
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 6; i++)
	{
		if (InCircle(chest[i].x, chest[i].y, p_mx, p_my))
		{
			this->isSelect = TRUE;
<<<<<<< HEAD
			this->selectedChest = chest[i];	//ì¢Œí‘œê°’ì— ë”°ë¼ ìƒìžì„ íƒí•˜ê¸°
			printf("%dë²ˆ ìƒìž ì„ íƒ\n", selectedChest.GetChestID());
=======
			this->selectedChest = chest[i];	//ÁÂÇ¥°ª¿¡ µû¶ó »óÀÚ¼±ÅÃÇÏ±â
			printf("%d¹ø »óÀÚ ¼±ÅÃ\n", selectedChest.GetChestID());
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
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
<<<<<<< HEAD
			wsprintf(p_text, TEXT("%dì„± - %d%%"), i + 1, selectedChest.GetProb(i));
			if (i == 3)
			{
				wsprintf(p_text, TEXT("ë§ˆë²• - %d%%"), selectedChest.GetProb(i));
=======
			wsprintf(p_text, TEXT("%d¼º - %d%%"), i + 1, selectedChest.GetProb(i));
			if (i == 3)
			{
				wsprintf(p_text, TEXT("¸¶¹ý - %d%%"), selectedChest.GetProb(i));
>>>>>>> 0264c57aa6c6a2dbd9f7563d09fb1056bad0373f
			}
			TextOut(p_hdc, 100 + i * 100, 675, p_text, lstrlen(p_text));
		}
		wsprintf(p_text, TEXT("%d"), selectedChest.GetChestID());
		TextOut(p_hdc, 100, 625, p_text, lstrlen(p_text));
	}
}