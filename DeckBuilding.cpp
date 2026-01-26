#include "DeckBuilding.h"

double LenghtPts(int x1, int y1, int x2, int y2)
{
	return sqrt((float)((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
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
			cout << to_string(i) << "번째 카드저장완료" << endl;
		}

		j["Cards_I"] = json::array();
		for (int i = 0; i < inven.size(); i++)
		{
			json card;
			card["uid"] = inven[i].GetUid();
			j["Cards_I"].push_back(card);
			cout << to_string(i) << "번째 카드저장완료" << endl;
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
			c.x = x * 120 + 460, c.y = y * 140 + 80;

			myDeck.push_back(c);
			++index;
		}
	}

	//키에 값이 존재하고 && 
	inven.clear();
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
			c.x = x * 82 + 1050, c.y = y * 120 + 130;

			inven.push_back(c);
			++index;
		}
	}

	std::cout << "Deck.json 로드 완료. Star_n: "
		<< Star_n[0] << "," << Star_n[1] << "," << Star_n[2] << " Cards_D: " << myDeck.size() << "\n";
	
}

vector<Card> DeckBuilding::EraseDuple(vector<Card> p_cards)
{
	//int psize = p_cards.size();
	for (int i = 0; i < p_cards.size(); i++)
	{
		if (p_cards[i].GetUid() < 0)
		{
			p_cards.erase(p_cards.begin() + i);
			for (int j = i; j < p_cards.size(); j++)
			{
				p_cards[j].x -= 82;
				if (p_cards[j].x < 1000)
				{
					p_cards[j].x = 82 * 4 + 1050, p_cards[j].y -= 130;
				}
			}
			--i;
		}
	}
	return p_cards;
}

int DeckBuilding::GetSize()
{
	return this->inven.size();
}

void DeckBuilding::PushCard(vector<Card> p_cards)
{
	//인벤에 들어갈 자리에 맞게 좌표를 세팅함.
	for (int i = 0; i < p_cards.size(); i++)
	{
		int x = (inven.size() + i) % 5, y = (inven.size() + i) / 5;
		p_cards[i].x = x * 82 + 1050, p_cards[i].y = y * 120 + 130;
	}
	//좌표 세팅 후 중복을 제거하고
	p_cards = EraseDuple(p_cards);
	//완성된 배열을 인벤에 추가
	inven.insert(inven.end(), p_cards.begin(), p_cards.end());
}

void DeckBuilding::ItoD(int p_mx, int p_my)
{
	for (int i = 0; i < inven.size(); i++)
	{
		//카드가 클릭되었을경우
		if (InCircle(inven[i].x, inven[i].y, p_mx, p_my))
		{
			Card selectedCard = inven[i];
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
			inven[i].x = x * 120 + 460, inven[i].y = y * 140 + 80;
			myDeck.push_back(inven[i]);
			inven.erase(inven.begin() + i);
			SelectedCard = &myDeck[myDeck.size() - 1];

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < inven.size(); j++)
			{
				inven[j].x -= 82;
				if (inven[j].x < 1000)
				{
					inven[j].x = 82 * 4 + 1050, inven[j].y -= 130;
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

			//이동시킬 카드의 좌표를 변경. 출발지 배열에서 제거 후 목적지 배열 맨 뒤에 추가
			int x = inven.size() % 5, y = inven.size() / 5;
			myDeck[i].x = x * 82 + 1050, myDeck[i].y = y * 120 + 130;
			inven.push_back(myDeck[i]);
			myDeck.erase(myDeck.begin() + i);
			SelectedCard = &inven[inven.size() - 1];

			//이동된 카드의 뒤쪽 카드들을 앞으로 한칸씩 좌표를 변경
			for (int j = i; j < myDeck.size(); j++)
			{
				myDeck[j].x -= 120;
				if (myDeck[j].x < 400)
				{
					myDeck[j].x = 120 * 4 + 460, myDeck[j].y -= 140;
				}
			}
			return;
		}
	}
}

void DeckBuilding::SelectCard(int p_mx, int p_my)
{
	//배열을 일일이 확인하는데 더 좋은방법이 없는가
	for (int i = 0; i < inven.size(); i++)
	{
		if (InCircle(inven[i].x, inven[i].y, p_mx, p_my))
		{
			SelectedCard = &inven[i];
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
		if (InCircle(1085, 690, p_mx, p_my)) this->PageBuff(false);
		else if (InCircle(1345, 690, p_mx, p_my)) this->PageBuff(true);
	}
}

void DeckBuilding::DrawInventory(HDC p_hdc, WCHAR p_text[])
{
	for (int i = 0; i < inven.size() + 1; i++)
	{
		//마지막카드이동시 배열에서 사라진카드이므로 예외처리
		m_rend.RemoveIDIamage("inven_card" + to_string(i));
	}
	for (int i = 0; i < 25; i++)
	{
		if (i + i_page * 25 >= inven.size()) return;
		m_rend.RemoveIDIamage("inven_card" + to_string(i));

		wstring image_path = L"card";
		wstring image_type;
		wstring image_star = to_wstring((int)inven[i + i_page * 25].GetStar() + 1);
		if (inven[i + i_page * 25].GetType() == E_Attack) image_type = L"_atk_";
		else if (inven[i + i_page * 25].GetType() == E_Deffense) image_type = L"_def_";//1040+82-30,50+140-30 -> x==1032+82,y==30+140
		image_path += image_type + image_star + L".png";				//1040-30,50-30 -> x-38,y-50 / x==1032,y==30

		m_rend.SetImage(image_path, "inven_card" + to_string(i), Rect(0, 0, 100, 132)
			, Rect(inven[i + i_page * 25].x - 38, inven[i + i_page * 25].y - 50, (int)(100 * 0.75f), (int)(132 * 0.75f))
			, true, GameImage_M::LayerType::Background);

		wsprintf(p_text, TEXT("%d"), inven[i + i_page * 25].GetUid());
		TextOut(p_hdc, inven[i].x - 10, inven[i + i_page * 25].y, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawMyDeck(HDC p_hdc, WCHAR p_text[])
{
	//마지막카드이동시 배열에서 사라진카드이므로 예외처리
	m_rend.RemoveIDIamage("deck_card" + to_string(myDeck.size()));
	for (int i = 0; i < myDeck.size(); i++)
	{
		m_rend.RemoveIDIamage("deck_card" + to_string(i));

		wstring image_path = L"card";
		wstring image_type;
		wstring image_star = to_wstring((int)myDeck[i].GetStar() + 1);
		if (myDeck[i].GetType() == E_Attack) image_type = L"_atk_";
		else if (myDeck[i].GetType() == E_Deffense) image_type = L"_def_";
		image_path += image_type + image_star + L".png";

		m_rend.SetImage(image_path, "deck_card" + to_string(i), Rect(0, 0, 100, 132)
			, Rect(myDeck[i].x - 50, myDeck[i].y - 66, 100, 132), true, GameImage_M::LayerType::Background);

		wsprintf(p_text, TEXT("%d"), myDeck[i].GetUid());
		TextOut(p_hdc, myDeck[i].x - 10, myDeck[i].y, p_text, lstrlen(p_text));
	}
}

void DeckBuilding::DrawDeckBuild(HDC p_hdc, WCHAR p_text[])
{
	DrawInventory(p_hdc, p_text);	//인벤
	DrawMyDeck(p_hdc, p_text);	//마이덱

	m_rend.RemoveIDIamage("s_card");
	if (SelectedCard)
	{
		wstring path = L"card";
		if (SelectedCard->GetType() == E_Attack) path += L"_atk_";
		else if (SelectedCard->GetType() == E_Deffense) path += L"_def_";
		path += to_wstring(SelectedCard->GetStar() + 1) + L".png";
		m_rend.SetImage(path, "s_card", Rect(0, 0, 100, 132), Rect(25, 100, 100 * 3.5f, 132 * 3.5f)
			, true, GameImage_M::LayerType::Background);
		m_rend.MoveImage("s_card", Rect(25, 100, 100 * 3.5f, 132 * 3.5f));
		wsprintf(p_text, TEXT("%d"), SelectedCard->GetUid());
		TextOut(p_hdc, 200, 350, p_text, lstrlen(p_text));
	}

	Rectangle(p_hdc, 1020, 670, 1150, 710);//인벤좌로이동
	Rectangle(p_hdc, 1280, 670, 1410, 710);//인벤우로이동

	//카드설명화면
	MoveToEx(p_hdc, 400, 0, NULL);
	LineTo(p_hdc, 400, 720);
	//마이덱
	MoveToEx(p_hdc, 1000, 0, NULL);
	LineTo(p_hdc, 1000, 720);
	//인벤토리
}
