#pragma once
//#include "MainGame.h"
#include "DeckBuilding.h"
#include "RenderManager.h"

//상자의 정보를 담음(확률, 좌표)
class Chest
{
private:
	int probability[4];			//1성 / 2성 / 3성 / 마법 별 등장확률 변수(총합100)
	int id;
	int price;
public:
	int x, y;

public:
	Chest()
	{

	}
	~Chest()
	{

	}

	//상자구별용id / 1상자 가격 / 1성 / 2성 / 3성 / 마법 별 등장확률 지정(확률총합100, 등장안하면 0)
	void SetChest(int p_id, int p_price, int p_1Star, int p_2Star, int p_3Star, int p_magic)
	{
		int chance_check = p_1Star + p_2Star + p_3Star + p_magic;
		//총합 100이 아닐시 설정취소
		if (chance_check != 100)
		{
			return;
		}

		probability[0] = p_1Star; probability[1] = p_2Star;
		probability[2] = p_3Star; probability[3] = p_magic;
		id = p_id, price = p_price;
		return;
	}

	//원하는 확률 하나를 리턴함
	int GetProb(int p_idx)
	{
		return this->probability[p_idx];
	}

	int GetChestID()
	{
		return this->id;
	}

	int GetPrice()
	{
		return this->price;
	}
};

class Shop
{
private:
	Chest chest[6];					//전체 상자
	Chest selectedChest;			//선택된 상자
	BOOL isSelect = FALSE;			//하이라이트될 상자를 선택했는가
public:
	Shop()
	{
		srand(time(NULL));
		for (int i = 0; i < 6; i++)//0 1 2 3 4 5
		{
			//테스트용 랜덤확률지정
			int hund = 101, v1 = 0, v2 = 0, v3 = 0, v4 = 0;
			v1 = rand() % hund, hund -= v1;	//v1 = 0~100 -> 12, hund = 88
			v2 = rand() % hund, hund -= v2;//v2 = 0~88 -> 20, hund = 68
			v3 = rand() % hund, hund -= v3, v4 = hund - 1;//v3 = 0~68 -> 40, hund = 28
			chest[i].SetChest(i, rand() % 100, v1, v2, v3, v4);

			//상자 위치 설정해줌
			chest[i].x = 875 + (i % 3) * 150, chest[i].y = 250 + (i / 3) * 200;
		}

		//상자 보관용 선반
		g_renderManager.SetImage(L"shelf.png", "shelf1"
			, Rect(0, 0, 651, 101), Rect(700, 250, 651, 101), false);
		g_renderManager.SetImage(L"shelf.png", "shelf2"
			, Rect(0, 0, 651, 101), Rect(700, 450, 651, 101), false);
		//상점 주인
		g_renderManager.SetImage(L"cookie.png", "cookie"
			, Rect(0, 0, 2500, 2500), Rect(300 - 180, 430 - 180, 360, 360), false);
		//상자 정보 하이라이트 및 상점 주인 대사 출력용
		g_renderManager.SetImage(L"textbox.png", "textbox"
			, Rect(0, 0, 500, 200), Rect(50, 500, 500, 200), false);
		//상자들 출력
		for (int i = 0; i < 6; i++)
		{
			g_renderManager.SetImage(L"chest.png", "chest" + to_string(i)
				, Rect(0, 0, 1024, 1024), Rect(chest[i].x - 48, chest[i].y - 78, 128, 128), false);
		}
	}
	virtual ~Shop()
	{

	}

	//상자를 선택함
	void SelectChest(int p_mx, int p_my);
	int GetSelectedPrice()
	{
		return selectedChest.GetPrice();
	}
	Chest GetSelectedChest()
	{
		return this->selectedChest;
	}
	BOOL CheckIsSelection();
	void CancelSelection();

	//이미지 로드 확인해보기 / 쿠키는 맥도날드 점원, 상자는 가게메뉴로 변경(==상점 컨셉을 맥으로)
	void SetDrawShop()
	{
		//상자 보관용 선반
		g_renderManager.ImageVisible("shelf1", true);
		g_renderManager.ImageVisible("shelf2", true);

		//상점 주인
		g_renderManager.ImageVisible("cookie", true);

		//상자 정보 하이라이트 및 상점 주인 대사 출력용
		g_renderManager.ImageVisible("textbox", true);

		//상자들 출력
		for (int i = 0; i < 6; i++)
		{
			g_renderManager.ImageVisible("chest" + to_string(i), true);
		}
	}
	//화면 전환 시 상점의 이미지들을 전부 제거
	void ClearShop()
	{
		g_renderManager.ImageVisible("shelf1", false);
		g_renderManager.ImageVisible("shelf2", false);
		g_renderManager.ImageVisible("cookie", false);
		g_renderManager.ImageVisible("textbox", false);
		for (int i = 0; i < 6; i++)
		{
			g_renderManager.ImageVisible("chest" + to_string(i), false);
		}
	}
	void DrawShop(HDC p_hdc, WCHAR p_text[]);
	void DrawEnterShop()
	{

	}
};

extern Shop g_shop;
