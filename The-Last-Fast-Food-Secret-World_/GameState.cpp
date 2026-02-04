#include "GameState.h"
#include "RenderManager.h"
#include "InputGame.h"
#include <chrono>

//각 화면
#include "Shop.h"
#include "DeckBuilding.h"
#include "CardGacha.h"
#include "MainGame.h"

namespace GameState_M {
	Context::Context() {
		StateVector.resize((int)E_InGameState::InGameResult + 1);
		StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		StateVector[(int)E_InGameState::Menu] = make_shared<Menu>();
		StateVector[(int)E_InGameState::DeckBuild] = make_shared<DeckBuild>();
		StateVector[(int)E_InGameState::Shop] = make_shared<Shop>();
		StateVector[(int)E_InGameState::LuckyBox] = make_shared<LuckyBox>();
		//StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		//StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		//StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
	}

	// 각 화면에 맞는 Update 로직
	void Context::Update(HDC p_hdc, HWND p_hwnd)
	{
		if (currentState == nullptr)
			return;

		currentState.get()->Update(p_hdc, p_hwnd);
	}

	void Context::ChangeState(E_InGameState inputState) {
		if (currentState != nullptr)
			currentState.get()->Exit();

		currentState = StateVector[(int)inputState];

		currentState.get()->Enter();
	}



	void Lobby::Enter()
	{
		g_DeckBuild.LoadDeck();//시작시 덱 로드, 세이브는 소멸자에
		//RENDER.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(0, 0, 300, 300)
			//, false, GameImage_M::LayerType::Field);
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {
		if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Left)
			STATE.ChangeState(GameState_M::E_InGameState::Shop);
	}

	void Lobby::Exit() {
		//RENDER.AllRemoveImage();
	}

	void Menu::Enter()
	{
		//RENDER.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(100, 0, 300, 300), true, GameImage_M::LayerType::Field);
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{

	}

	void Menu::Exit() {
		//RENDER.RemoveIDIamage("ID_1");
	}

	void DeckBuild::Enter() {
		//덱빌드진입
		g_DeckBuild.EnterDeckBuild();
	}

	void DeckBuild::Update(HDC p_hdc, HWND p_hwnd) {
		WCHAR text[256] = L"";

		//좌클릭시 카드 선택
		if (INPUT.isClick() == (int)GameInput_M::MouseValue::Left)
		{
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "rb1")
				STATE.ChangeState(GameState_M::E_InGameState::Shop);
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'L');
		}
		//우클릭시 카드 이동
		else if (INPUT.isClick() == (int)GameInput_M::MouseValue::Right)
		{
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'R');
		}

		//화면 그리기
		g_DeckBuild.DrawDeckBuild(p_hdc, text);
	}

	void DeckBuild::Exit() {
		//덱빌드 퇴장
		g_DeckBuild.ExitDeckBuild();
	}

	void Shop::Enter()
	{
		//상점 진입
		while (!g_Shop.DrawEnterShop())
		{
			chrono::steady_clock::time_point start = chrono::steady_clock::now();
			g_Shop.SetDrawShop();
		}


		chrono::steady_clock::time_point end = chrono::steady_clock::now();
		//SetTimer(hWnd, 1, 8, NULL);
		cout << "Shop Enter" << endl;
	}

	void Shop::Update(HDC p_hdc, HWND p_hwnd)
	{
		WCHAR text[256] = L"";
		//좌클릭시 상자선택
		if (INPUT.isClick() == (int)GameInput_M::MouseValue::Left)
		{
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			g_Shop.SelectChest(g_MainGame.mx, g_MainGame.my);
		}

		//상자가 선택된 경우 -> 뽑기버튼 활성화 및 뽑기화면전환
		if (g_Shop.CheckIsSelection())
		{
			if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "one" && g_Shop.isSucceedGacha(p_hdc, 1))
			{
				g_Gacha.GetGacha(TRUE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());
				STATE.ChangeState(GameState_M::E_InGameState::LuckyBox);
			}
			else if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "ten" && g_Shop.isSucceedGacha(p_hdc, 10))
			{
				g_Gacha.GetGacha(FALSE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());
				STATE.ChangeState(GameState_M::E_InGameState::LuckyBox);
			}
			g_Shop.DrawGachaButton(p_hdc, g_MainGame.mx, g_MainGame.my, text);
		}
		
		//상점 화면 그리기
		g_Shop.DrawShop(p_hdc, text);

		//임시 - 우클릭 시 덱빌딩화면 전환
		if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Right)
			STATE.ChangeState(GameState_M::E_InGameState::DeckBuild);
	}

	void Shop::Exit()
	{
		//상점 퇴장
		g_Shop.ExitShop();
	}

	void LuckyBox::Enter()
	{
		g_Gacha.EnterGacha();
	}

	void LuckyBox::Update(HDC p_hdc, HWND p_hwnd)
	{
		WCHAR text[256] = L"";
		//좌클릭시 좌표 받아오고
		if (INPUT.isClick() == (int)GameInput_M::MouseValue::Left)
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);

		//돌아가기 버튼 클릭시 상점으로 이동
		if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "back")
		{
			STATE.ChangeState(GameState_M::E_InGameState::Shop);
			return;
		}

		g_Gacha.InGacha();//뽑은카드 좌표 세팅
		g_Gacha.DrawGacha(p_hdc, g_MainGame.mx, g_MainGame.my, text);//뽑은카드 그리기
	}

	void LuckyBox::Exit()
	{
		cout << "exit" << endl;
		g_Gacha.ExitGacha();
	}

	void InGame::Enter()
	{
	}

	void InGame::Update(HDC p_hdc, HWND p_hwnd)
	{
	}

	void InGame::Exit()
	{
	}

	void InGameResult::Enter()
	{
	}

	void InGameResult::Update(HDC p_hdc, HWND p_hwnd)
	{
	}

	void InGameResult::Exit()
	{
	}

}