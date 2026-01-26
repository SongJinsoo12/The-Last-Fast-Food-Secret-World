#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Card.h"
#include "macroNum.h"
#include "RenderManager.h"

#include <iostream>
#include <fstream>	//파일읽기
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

#define BSIZE 25
double LenghtPts(int x1, int y1, int x2, int y2);
BOOL InCircle(int x, int y, int mx, int my);

class DeckBuilding
{
private:
	//현재 덱
	vector<Card> myDeck;
	//보유한 카드들
	vector<Card> inven;
	Card* SelectedCard;//덱편집화면에서 유저가 정보를 확인할 카드변수 / exit시 선택해제할것

	int Star_n[3] = { 0, };//덱의 1,2,3성 카드개수
	int i_page = 0, max_page = 0;

public:
	DeckBuilding()
	{
		
	}
	virtual ~DeckBuilding()
	{
	}

	void SaveDeck();
	void LoadDeck();

	void PageBuff(bool p_isIncrease)
	{
		if (p_isIncrease)
		{
			++i_page;
			if (i_page > max_page) --i_page;
		}
		else
		{
			--i_page;
			if (i_page < 0) i_page = 0;
		}
		cout << i_page << " / " << max_page << endl;
		return;
	}
	//중복이 존재하면 제거후 뒤의 카드들을 앞으로 이동
	vector<Card> EraseDuple(vector<Card> p_cards);

	//인벤토리의 사이즈 리턴
	int GetSize();

	//파라미터의 배열을 인벤 맨 뒤에 추가함. 중복 제거도 해줌
	void PushCard(vector<Card> p_cards);

	//인벤->덱 (덱, 마우스X, 마우스Y)
	void ItoD(int p_mx, int p_my);
	//덱->인벤 (덱, 마우스X, 마우스Y)
	void DtoI(int p_mx, int p_my);
	void SelectCard(int p_mx, int p_my);
	void DeckBuild(int p_mx, int p_my, char click_m);//click_m == 좌/우클릭 확인용, 좌-카드하이라이트/우-카드이동

	//인벤토리 출력
	void DrawInventory(HDC p_hdc, WCHAR p_text[]);
	//마이덱 출력
	void DrawMyDeck(HDC p_hdc, WCHAR p_text[]);
	//덱빌딩 화면 출력
	void DrawDeckBuild(HDC p_hdc, WCHAR p_text[]);

	void EnterDeckBuild()
	{
		max_page = (inven.size() / 25);
		cout << "maxpage : " << max_page << endl;
	}

	void ExitDeckBuild()
	{

	}
};
