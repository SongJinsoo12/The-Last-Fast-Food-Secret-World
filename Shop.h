#pragma once
#include "MainGame.h"
#include "DeckBuilding.h"

class Chest
{
private:
	int probability[4];			//1성 / 2성 / 3성 / 마법 별 등장확률 지정(총합100)
	int x, y;

public:
	Chest();
	~Chest();
};

class Shop
{
private:
	bool isSelect = false;
	Chest chest[6];
	Chest outChest;
public:
	Shop();
	virtual ~Shop();

	Chest SelectChest(int p_mx, int p_my)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				if (InCircle(x * 100, y * 50, p_mx, p_my))
				{
					this->isSelect = true;
					outChest = chest[x * y];	//좌표값에 따라 상자선택하기
					return chest[x * y];
				}
			}
		}
	}

	void DrawShop(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[])
	{
		if (isSelect)
		{
			Rectangle(p_hdc, 50, 50, 200, 700);
		}
	}
};