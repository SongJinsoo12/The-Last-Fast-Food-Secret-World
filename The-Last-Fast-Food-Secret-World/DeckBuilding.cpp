#include "DeckBuilding.h"

DeckBuilding g_DeckBuild;

double LenghtPts(int x1, int y1, int x2, int y2)
{
	return sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

double LenghtPtsSqrt(int x1, int y1, int x2, int y2)
{
	return (float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

BOOL InCircle(int x, int y, int mx, int my)
{
	if (LenghtPts(x, y, mx, my) < BSIZE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void DeckBuilding::SaveDeck()//iscolleted::array
{
	{
		// 1. JSON 객체 생성 및 데이터 추가
		json j;//j["키"] = "키값"
		j["Star_n"] = { Star_n[0], Star_n[1], Star_n[2] };

		//배열형태로 "Cards_D"키 생성
		j["Cards_D"] = json::array();
		for (int i = 0; i < myDeck.size(); i++)
		{
			//"uid"키에 Uid값을 연결하고 "Cards_D"키 배열 맨뒤에 저장
			json card;
			card["uid"] = myDeck[i].GetUid();
			j["Cards_D"].push_back(card);
			//cout << to_string(i) << "번째 카드저장완료" << endl;
		}

		j["Cards_I"] = json::array();
		for (int i = 0; i < inven_list[0].size(); i++)
		{
			json card;
			card["uid"] = inven_list[0][i].GetUid();
			j["Cards_I"].push_back(card);
			//cout << to_string(i) << "번째 카드저장완료" << endl;
		}

		// 2. 파일 스트림 열기 (output.json 파일)
		std::ofstream outfile("Deck.json");

		// 3. JSON 객체를 파일 스트림에 출력 (직렬화)
		if (outfile.is_open())
		{
			outfile << j.dump(4) << endl; // << 연산자로 파일에 쓰기
			outfile.close();
			std::cout << "JSON 데이터가 output.json 파일에 저장되었습니다." << std::endl;
		}
		else
		{
			std::cerr << "파일 열기 실패!" << std::endl;
			return;
		}
	}
}

void DeckBuilding::LoadDeck()
{

	std::ifstream file("Deck.json", std::ios::in);//같은 경로내에서 파일불러오기

	//파일 불러오기확인용
	if (!file.is_open())
	{
		std::cerr << "Error opening Deck.json.\n";
		return;
	}

	//읽어들인 파일을 json에 저장
	json j;
	try
	{
		file >> j;
	}
	catch (const std::exception& e)
	{
		cerr << "json parcing error : " << e.what() << "\n";
		return;
	}

	//"Star_n"키에 값이 존재할경우 불러오기
	if (j.contains("Star_n") && j["Star_n"].is_array() && j["Star_n"].size() >= 3)
	{
		for (int i = 0; i < 3; i++) Star_n[i] = j["Star_n"][i].get<int>();
	}
	else
	{
		Star_n[0] = 0, Star_n[1] = 0, Star_n[2] = 0;
	}

	//
	myDeck.clear();
	if (j.contains("Cards_D") && j["Cards_D"].is_array())
	{
		int index = 0;
		//"Cards_D" 배열이 끝날때까지 반복
		for (const auto& card : j["Cards_D"])
		{
			int uid = -1;
			//uid키에 값이 존재하면 int로 받아옴
			if (card.contains("uid"))
			{
				try { uid = card["uid"].get<int>(); }
				catch (...) { uid = -1; }
			}

			Card c;
			if (uid >= 0) c = Card(uid);
			else c = Card();

			// 좌표 복원: 인게임에서 사용하는 위치 규칙과 동일하게 세팅
			int x = index % 5, y = index / 5;
			c.x = x * 110 + 406, c.y = y * 130 + 80;

			myDeck.push_back(c);
			++index;
		}
	}

	//키에 값이 존재하고 && 
	inven_list[0].clear();
	if (j.contains("Cards_I") && j["Cards_I"].is_array())
	{
		int index = 0;
		for (const auto& card : j["Cards_I"])
		{
			int uid = -1;
			if (card.contains("uid"))
			{
				try { uid = card["uid"].get<int>(); }
				catch (...) { uid = -1; }
			}

			Card c;
			if (uid >= 0) c = Card(uid);
			else c = Card();

			// 좌표 복원: 인게임에서 사용하는 위치 규칙과 동일하게 세팅
			int x = (index % 25) % 5, y = (index % 25) / 5;
			c.x = x * 72 + 950, c.y = y * 110 + 130;

			inven_list[0].push_back(c);
			++index;
		}
	}

	std::cout << "Deck.json 로드 완료. Star_n: "
		<< Star_n[0] << "," << Star_n[1] << "," << Star_n[2] << " Cards_D: " << myDeck.size() << "\n";

}

void DeckBuilding::PageBuff(bool p_isIncrease)
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

vector<Card> DeckBuilding::EraseDuple(vector<Card> p_cards)
{
	for (int i = 0; i < p_cards.size(); i++)
	{
		if (p_cards[i].GetUid() < 0)
		{
			p_cards.erase(p_cards.begin() + i);
			for (int j = i; j < p_cards.size(); j++)
			{
				p_cards[j].x -= 72;
				if (p_cards[j].x < 900)
				{
					p_cards[j].x = 72 * 4 + 950, p_cards[j].y -= 110;
					if (p_cards[j].y < 120)
					{
						p_cards[j].x = 72 * 4 + 950, p_cards[j].y = 110 * 4 + 130;
					}
				}
			}
			--i;
		}
	}
	return p_cards;
}

int DeckBuilding::GetSize()
{
	return this->inven_list[0].size();
}

void DeckBuilding::PushCard(vector<Card> p_cards)
{
	//완성된 배열을 인벤에 추가
	inven_list[0].insert(inven_list[0].end(), p_cards.begin(), p_cards.end());
	inven_list[0] = SortCards(inven_list[0]);
	inven_list[0] = SetPos(inven_list[0]);
	//좌표 세팅 후 중복을 제거하고
	inven_list[0] = EraseDuple(inven_list[0]);
}

void DeckBuilding::ItoD(int p_mx, int p_my)
{
	for (int i = 0; i < inven_list[0].size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(inven_list[0][i].x, inven_list[0][i].y, p_mx, p_my))
		{
			Card selectedCard = inven_list[0][i];
			//덱이 꽉 차면 종료
			if (myDeck.size() >= DECKMAXSIZE)
			{
				cout << "덱 꽉참" << "\n";
				return;
			}
			////덱에 들어갈 수 있는 1성이 최대라면 종료
			//else if (Star_n[0] >= 15)
			//{
			//	cout << "1성 꽉참" << "\n";
			//	return;
			//}
			////덱에 들어갈 수 있는 2성이 최대라면 종료
			//else if (Star_n[1] >= 7)
			//{
			//	cout << "2성 꽉참" << "\n";
			//	return;
			//}
			////덱에 들어갈 수 있는 3성이 최대라면 종료
			//else if (Star_n[2] >= 3)
			//{
			//	cout << "3성 꽉참" << "\n";
			//	return;
			//}

			int star = selectedCard.GetStar();
			++Star_n[star];
			cout << Star_n[0] << Star_n[1] << Star_n[2] << endl;

			//(val * 간격 + 젤(왼 / 위)쪽으로부터의 여백)
			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = myDeck.size() % 5, y = myDeck.size() / 5;
			inven_list[0][i].x = x * 110 + 406, inven_list[0][i].y = y * 130 + 80;
			myDeck.push_back(inven_list[0][i]);
			inven_list[0].erase(inven_list[0].begin() + i);
			SelectedCard = &myDeck[myDeck.size() - 1];

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < inven_list[0].size(); j++)
			{
				inven_list[0][j].x -= 72;
				if (inven_list[0][j].x < 900)
				{
					inven_list[0][j].x = 72 * 4 + 950, inven_list[0][j].y -= 110;
					if (inven_list[0][j].y < 120) inven_list[0][j].x = 72 * 4 + 950, inven_list[0][j].y = 110 * 4 + 130;
				}
			}
			return;
		}
	}
}

void DeckBuilding::DtoI(int p_mx, int p_my)
{
	for (int i = 0; i < myDeck.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(myDeck[i].x, myDeck[i].y, p_mx, p_my))
		{
			Card selectedCard = myDeck[i];
			int star = selectedCard.GetStar();
			--Star_n[star];
			cout << Star_n[0] << Star_n[1] << Star_n[2] << endl;

			//마지막카드이동시 배열에서 사라진카드이므로 예외처리
			RENDER.ImageVisible(to_string(myDeck[i].GetUid()), false);
			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = (inven_list[0].size() % 25) % 5, y = (inven_list[0].size() % 25) / 5;
			myDeck[i].x = x * 72 + 950, myDeck[i].y = y * 110 + 130;
			inven_list[0].push_back(myDeck[i]);
			myDeck.erase(myDeck.begin() + i);
			SelectedCard = &inven_list[0][inven_list[0].size() - 1];

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < myDeck.size(); j++)
			{
				myDeck[j].x -= 110;
				if (myDeck[j].x < 350)
				{
					myDeck[j].x = 110 * 4 + 406, myDeck[j].y -= 130;
				}
			}
			return;
		}
	}
}

void DeckBuilding::SelectCard(int p_mx, int p_my)
{
	//배열을 일일이 확인하는데 더 좋은방법이 없는가
	for (int i = 0; i < inven_list[0].size(); i++)
	{
		if (InCircle(inven_list[0][i].x, inven_list[0][i].y, p_mx, p_my))
		{
			SelectedCard = &inven_list[0][i];
			return;
		}
	}
	for (int i = 0; i < myDeck.size(); i++)
	{
		if (InCircle(myDeck[i].x, myDeck[i].y, p_mx, p_my))
		{
			SelectedCard = &myDeck[i];
			return;
		}
	}
}

void DeckBuilding::DeckBuild(int p_mx, int p_my, char click_m)
{
	if (click_m == 'R')
	{
		this->ItoD(p_mx, p_my);	//인벤->마이덱
		this->DtoI(p_mx, p_my);	//마이덱->인벤
	}
	else if (click_m == 'L')
	{
		this->SelectCard(p_mx, p_my);
		/*if (InCircle(985, 690, p_mx, p_my)) 
		else if (InCircle(1245, 690, p_mx, p_my)) this->PageBuff(true);
		else if (InCircle(965, 45, p_mx, p_my)) this->ChangeFilter();*/
		if (btnManager.HandleClickId(p_mx, p_my) == "left") this->PageBuff(false);
		else if (btnManager.HandleClickId(p_mx, p_my) == "right") this->PageBuff(true);
		else if (btnManager.HandleClickId(p_mx, p_my) == "filter") this->ChangeFilter();
	}
}

void DeckBuilding::EnterDeckBuild()
{
	filter = 0;
	for (int i = 0; i < inven_list[0].size(); i++)
	{
		pushTypeCard(inven_list[0][i]);
	}

	inven_list[1] = SetPos(inven_list[1]);
	inven_list[2] = SetPos(inven_list[2]);
	inven_list[3] = SetPos(inven_list[3]);

	max_page = (inven_list[0].size() / 25);
	cout << "maxpage : " << max_page << endl;
	DrawOnly_pre = inven_list[0];

	RENDER.SetImage(L"rect_button.png", "rb1"
		, Rect(0, 0, 100, 101), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
	btnManager.AddButton(make_shared<RectButton>("rb1", RECT{ 10, 10, 100, 60 }));
}

void DeckBuilding::ExitDeckBuild()
{
	for (int i = 0; i < inven_list[filter].size(); i++) RENDER.ImageVisible(to_string(inven_list[filter][i].GetUid()), false);
	for (int i = 0; i < myDeck.size(); i++) RENDER.ImageVisible(to_string(myDeck[i].GetUid()), false);
	RENDER.RemoveIDIamage("s_card");
	RENDER.RemoveIDIamage("rb1");
	RENDER.RemoveIDIamage("left");
	RENDER.RemoveIDIamage("right");
	RENDER.RemoveIDIamage("filter");
	filter = 0;
}

void DeckBuilding::DrawInventory(HDC p_hdc, WCHAR p_text[], vector<Card> p_cardType)
{
	for (int i = 0; i < DrawOnly_pre.size(); i++)
	{
		//마지막카드이동시 배열에서 사라진카드이므로 예외처리
		RENDER.ImageVisible(to_string(DrawOnly_pre[i].GetUid()), false);
	}
	for (int i = 0; i < 25; i++)
	{
		int index = i + i_page * 25;
		if (index >= p_cardType.size())
		{
			DrawOnly_pre = p_cardType;
			return;
		}

		RENDER.MoveImage(to_string(p_cardType[index].GetUid()), Rect(p_cardType[index].x - 38, p_cardType[index].y - 50, (int)(100 * 0.55f), (int)(132 * 0.55f)));
		RENDER.ImageVisible(to_string(p_cardType[index].GetUid()), true);
	}
}

void DeckBuilding::DrawMyDeck(HDC p_hdc, WCHAR p_text[])
{
	int decksize = myDeck.size();

	for (int i = 0; i < decksize; i++)
	{
		RENDER.ImageVisible(to_string(myDeck[i].GetUid()), false);
	}
	
	for (int i = 0; i < decksize; i++)
	{
		RENDER.MoveImage(to_string(myDeck[i].GetUid()), Rect(myDeck[i].x - 50, myDeck[i].y - 66, (int)(100 * 0.9f), (int)(132 * 0.9f)));
		RENDER.ImageVisible(to_string(myDeck[i].GetUid()), true);
	}
}

void DeckBuilding::DrawDeckBuild(HDC p_hdc, WCHAR p_text[])
{
	DrawInventory(p_hdc, p_text, inven_list[filter]); //필터에 따른 인벤 출력
	DrawMyDeck(p_hdc, p_text);	//마이덱

	RENDER.ImageVisible("rb1", true);
	RENDER.RemoveIDIamage("s_card");
	if (SelectedCard)
	{
		wstring path = L"card";
		if (SelectedCard->GetType() == E_Attack) path += L"_atk_";
		else if (SelectedCard->GetType() == E_Deffense) path += L"_def_";
		path += to_wstring(SelectedCard->GetStar() + 1) + L".png";
		if (SelectedCard->GetType() == E_Magic) path = L"card_magic.png";
		RENDER.SetImage(path, "s_card", Rect(0, 0, 100, 132), Rect(25, 130, (int)(100 * 3.0f), (int)(132 * 3.0f))
			, true, GameImage_M::LayerType::Background);
	}

	RENDER.SetImage(L"rect_button.png", "left"
		, Rect(0, 0, 100, 101), Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
	btnManager.AddButton(make_shared<RectButton>("left", RECT{ 920, 620, 1030, 660 }));
	//Rectangle(p_hdc, 920, 620, 1030, 660);//인벤좌로이동
	//wsprintf(p_text, TEXT("←"));
	//TextOut(p_hdc, 985, 680, p_text, lstrlen(p_text));

	RENDER.SetImage(L"rect_button.png", "right"
		, Rect(0, 0, 100, 101), Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
	btnManager.AddButton(make_shared<RectButton>("right", RECT{ 1140, 620, 1250, 660 }));
	//Rectangle(p_hdc, 1140, 620, 1250, 660);//인벤우로이동
	//wsprintf(p_text, TEXT("→"));
	//TextOut(p_hdc, 1245, 680, p_text, lstrlen(p_text));

	wsprintf(p_text, TEXT("%d / %d"), i_page + 1, max_page + 1);
	TextOut(p_hdc, 1105, 680, p_text, lstrlen(p_text));

	//카드설명화면--
	MoveToEx(p_hdc, 350, 0, NULL);
	LineTo(p_hdc, 350, 720);
	//--마이덱--
	MoveToEx(p_hdc, 900, 0, NULL);
	LineTo(p_hdc, 900, 720);
	//--인벤토리

	RENDER.SetImage(L"rect_button.png", "filter"
		, Rect(0, 0, 100, 101), Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
	btnManager.AddButton(make_shared<RectButton>("filter", RECT{ 915, 20, 1065, 70 }));
	//Rectangle(p_hdc, 915, 20, 1065, 70);
	wsprintf(p_text, TEXT("%d"), filter);
	TextOut(p_hdc, 925, 40, p_text, lstrlen(p_text));

	//Rectangle(p_hdc, 915 + 150, 20, 1115 + 150, 70);

	Rectangle(p_hdc, 915 + 180, 20, 1065 + 180, 70);
	wsprintf(p_text, TEXT("?"));
	TextOut(p_hdc, 960 + 180, 40, p_text, lstrlen(p_text));

}