#pragma once
#include "MainGame.h"
#include "DeckBuilding.h"

//상자의 정보를 담음(확률, 좌표)
class Chest
{
private:
	int probability[4];			//1성 / 2성 / 3성 / 마법 별 등장확률 변수(총합100)
	int id;
public:
	int x, y;

public:
	Chest()
	{

	}
	~Chest()
	{

	}

	//상자구별용id / 1성 / 2성 / 3성 / 마법 별 등장확률 지정(확률총합100, 등장안하면 0)
	void SetChest(int p_id, int p_1Star = 0, int p_2Star = 0, int p_3Star = 0, int p_magic = 0)
	{
		int chance_check = p_1Star + p_2Star + p_3Star + p_magic;
		//총합 100이 아닐시 설정취소
		if (chance_check != 100)
		{
			return;
		}

		probability[0] = p_1Star;
		probability[1] = p_2Star;
		probability[2] = p_3Star;
		probability[3] = p_magic;

		id = p_id;
		return;
	}

	//원하는 확률 하나를 리턴함
	int GetProb(int p_idx)
	{
		return probability[p_idx];
	}

	int GetChestID()
	{
		return id;
	}
};

class Shop
{
protected:
	BOOL isSelect = FALSE;			//하이라이트될 상자를 선택했는가

private:
	Chest chest[6];					//전체 상자
	Chest selectedChest;			//선택된 상자
public:
	Shop()
	{
		srand(time(NULL));
		//상자 위치 설정해줌
		chest[0].x = 875, chest[0].y = 250;
		chest[1].x = 1025, chest[1].y = 250;
		chest[2].x = 1175, chest[2].y = 250;
		chest[3].x = 875, chest[3].y = 250 + 200;
		chest[4].x = 1025, chest[4].y = 250 + 200;
		chest[5].x = 1175, chest[5].y = 250 + 200;

		for (int i = 0; i < 6; i++)
		{
			//테스트용 랜덤확률지정
			int hund = 101, v1 = 0, v2 = 0, v3 = 0, v4 = 0;
			v1 = rand() % hund, hund -= v1;	//v1 = 0~100 -> 12, hund = 88
			v2 = rand() % hund, hund -= v2;//v2 = 0~88 -> 20, hund = 68
			v3 = rand() % hund, hund -= v3, v4 = hund - 1;//v3 = 0~68 -> 40, hund = 28
			chest[i].SetChest(i, v1, v2, v3, v4);
		}
	}
	virtual ~Shop()
	{

	}

	//상자를 선택함
	void SelectChest(int p_mx, int p_my);

	void DrawShop(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};