#pragma once
#include <cstdlib>
#include <vector>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Card.h"
#include "macroNum.h"

#include <iostream>
#include <fstream>	//파일읽기
#include <map>		//키-키값
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

	int Star_n[3] = { 0, };//덱의 1,2,3성 카드개수

public:
	DeckBuilding()
	{

	}

	virtual ~DeckBuilding()
	{

	}

	void SaveDeck()
	{
		// 1. JSON 객체 생성 및 데이터 추가
		json j;
		j[]
		for (int i = 0; i < myDeck.size(); i++)
		{
			int id = myDeck[i].GetUid();
			j["ID" + to_string(i)] = to_string(id);
			cout << to_string(i) << "번째 카드저장완료" << endl;
		}

		// 2. 파일 스트림 열기 (output.json 파일)
		std::ofstream outfile("Deck.json");

		// 3. JSON 객체를 파일 스트림에 출력 (직렬화)
		if (outfile.is_open()) {
			outfile << j << std::endl; // << 연산자로 파일에 쓰기
			outfile.close();
			std::cout << "JSON 데이터가 output.json 파일에 저장되었습니다." << std::endl;
		}
		else {
			std::cerr << "파일 열기 실패!" << std::endl;
		}
	}

	void LoadDeck()
	{
		std::ifstream file("Deck.json", std::ios::in);//같은 경로내에서 파일불러오기

		//파일 불러오기확인용
		if (!file.is_open())
		{
			std::cerr << "Error opening Deck.txt.\n";
			return;
		}

		//읽어들인 파일을 json에 저장
		json j;
		file >> j;

		//map배열에 불러온 키와 키값을 저장함
		std::map<std::string, std::string> Card;
		for (const auto& result : j["results"])
			Card[result["ID"].get<std::string>()] = result["Value"].get<std::string>();

		//first == 키이름, second == 키값
		for (const auto& pair : Card)
			std::cout << pair.first << " : " << pair.second << "\n";
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
	void DeckBuild(int p_mx, int p_my);

	//인벤토리 출력
	void DrawInventory(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//마이덱 출력
	void DrawMyDeck(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
	//덱빌딩 화면 출력
	void DrawDeckBuild(HDC p_hdc, HPEN p_hpen, HPEN p_oldpen, int p_mx, int p_my, WCHAR p_text[]);
};

extern DeckBuilding g_DeckBuilding;
