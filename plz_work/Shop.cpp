#include "Shop.h"

Shop g_Shop;

bool Shop::isSucceedGacha(HDC p_hdc, int p_gacha_num)
{
	int remove_gold = selectedChest.GetPrice() * p_gacha_num;
	if (g_MainGame.RemoveGold(remove_gold))
	{
		return true;
	}
	else
	{
		TextOut(p_hdc, 50, 500, TEXT("돈이 부족합니다."), 10);
		return false;
	}
}

void Shop::DrawGachaButton(HDC p_hdc, int p_mx, int p_my, WCHAR p_text[])
{
	//버튼생성위치 옮기고 여기서는 온오프만 관리
	//1뽑 버튼
	m_ShopBtnM.AddButton(make_shared<RectButton>("one", RECT{ 600, 595, 915, 665 }));
	wsprintf(p_text, TEXT("1개 - %dG"), selectedChest.GetPrice());
	TextOut(p_hdc, 720, 625, p_text, lstrlen(p_text));

	//10뽑 버튼
	m_ShopBtnM.AddButton(make_shared<RectButton>("ten", RECT{ 935, 595, 1250, 665 }));
	wsprintf(p_text, TEXT("10개 - %dG"), selectedChest.GetPrice() * 9);
	TextOut(p_hdc, 1055, 625, p_text, lstrlen(p_text));
}

//상자를 선택함
void Shop::SelectChest(int p_mx, int p_my)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_ShopBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "chest" + to_string(i))
		{
			RENDER.ImageVisible("one", true);
			RENDER.ImageVisible("ten", true);
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
	RENDER.ImageVisible("one", false);
	RENDER.ImageVisible("ten", false);
	this->isSelect = FALSE;
}

void Shop::SetDrawShop()
{
	RENDER.SetImage(L"caption.jpg", "bg", Rect(0, 0, 600, 400)
		, Rect(0, 0, 1280, 720), true, GameImage_M::LayerType::Background);

	//상자 보관용 선반
	RENDER.SetImage(L"shelf.png", "shelf1"
		, Rect(0, 0, 651, 101), Rect(mov_sel, 250, 651, 101), true, GameImage_M::LayerType::UI);
	RENDER.SetImage(L"shelf.png", "shelf2"
		, Rect(0, 0, 651, 101), Rect(mov_sel, 450, 651, 101), true, GameImage_M::LayerType::UI);

	//상자 정보 및 상점 주인 대사 출력용
	RENDER.SetImage(L"textbox.png", "textbox"
		, Rect(0, 0, 500, 200), Rect(50, 450, 500, 200), false, GameImage_M::LayerType::UI);


	RENDER.SetImage(L"rect_button.png", "one"
		, Rect(0, 0, 100, 110), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
	RENDER.SetImage(L"rect_button.png", "ten"
		, Rect(0, 0, 100, 110), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
}

void Shop::SetEnterShop()
{
	mov_sel = 1400;
	ani_start = chrono::steady_clock::now();
}

void Shop::ExitShop()
{
	RENDER.RemoveIDIamage("bg");
	RENDER.RemoveIDIamage("shelf1");
	RENDER.RemoveIDIamage("shelf2");
	RENDER.RemoveIDIamage("employee");
	RENDER.RemoveIDIamage("textbox");
	RENDER.RemoveIDIamage("one");
	RENDER.RemoveIDIamage("ten");
	RENDER.RemoveIDIamage("back");
	for (int i = 0; i < 3; i++) RENDER.RemoveIDIamage("chest" + to_string(i+1));
	//RENDER.AllRemoveImage();
	mov_sel = 1400;
	this->CancelSelection();
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
			TextOut(p_hdc, 100 + i * 100, 625, p_text, lstrlen(p_text));
		}
	}
	RENDER.SetImage(L"rect_button.png", "back", Rect(0, 0, 100, 101)
		, Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
	m_ShopBtnM.AddButton(make_shared<RectButton>("back", RECT{ 10, 50, 60, 70 }));

	m_ShopBtnM.DrawAll();
}

bool Shop::DrawEnterShop()
{
	if (mov_sel <= 710) return true;
	//MoveImageTween("shelf1", Rect(), 3).OnComplete( EndFN );
	auto now = std::chrono::steady_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - ani_start).count();

	RENDER.MoveImage("shelf1", Rect(mov_sel - 100, 250, 651, 101));
	RENDER.MoveImage("shelf2", Rect(mov_sel - 100, 450, 651, 101));
	RENDER.ImageVisible("textbox", true);
	RENDER.MoveImage("textbox", Rect((700 - mov_sel) + 50, 450, 500, 200));

	int dur = 5000;

	if (elapsed < dur)
	{
		int gap = (int)((mov_sel - 700) / 10);
		mov_sel -= gap;
		
		if (mov_sel <= 710)//입장 애니메이션이 끝난 후 나머지 이미지 활성화
		{
			//상자 출력
			for (int i = 0; i < 3; i++)
			{
				wstring path = L"chest_" + to_wstring(i+1) + L".png";
				RENDER.SetImage(path, "chest" + to_string(i+1)
					, Rect(0, 0, 300, 300), Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
				m_ShopBtnM.AddButton(make_shared<CircleButton>("chest" + to_string(i+1), chest[i].x - 85, chest[i].y - 15, 64));
			}

			//상점 주인 / 상호작용 추가하기
			RENDER.SetImage(L"employee.png", "employee"
				, Rect(0, 0, 250, 235), Rect(0, 0, 0, 0), true, GameImage_M::LayerType::UI);
			m_ShopBtnM.AddButton(make_shared<CircleButton>("employee", 280, 320, 180));
			return true;
		}
	}
	else
	{
		ani_start = std::chrono::steady_clock::now();
		return false;
	}
}