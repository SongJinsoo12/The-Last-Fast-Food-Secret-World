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
		//m_rend.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(0, 0, 300, 300)
			//, false, GameImage_M::LayerType::Field);
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {

		if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Left)
			m_State.ChangeState(GameState_M::E_InGameState::Shop);
	}

	void Lobby::Exit() {
		//m_rend.AllRemoveImage();
	}

	void Menu::Enter()
	{
		//m_rend.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(100, 0, 300, 300), true, GameImage_M::LayerType::Field);
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{

	}

	void Menu::Exit() {
		//m_rend.RemoveIDIamage("ID_1");
	}

	void DeckBuild::Enter() {
		g_DeckBuild.EnterDeckBuild();
	}

	void DeckBuild::Update(HDC p_hdc, HWND p_hwnd) {
		WCHAR text[256] = L"";

		if (m_Input.isClick() == (int)GameInput_M::MouseValue::Left)
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'L');
		else if (m_Input.isClick() == (int)GameInput_M::MouseValue::Right)
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'R');

		g_DeckBuild.DrawDeckBuild(p_hdc, text);
	}

	void DeckBuild::Exit() {
		g_DeckBuild.ExitDeckBuild();
	}

	void Shop::Enter()
	{
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
		cout << "Shop Update" << endl;
		if (m_Input.isClick() == (int)GameInput_M::MouseValue::Left)
		{
			m_Input.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			g_Shop.SelectChest(g_MainGame.mx, g_MainGame.my);
		}

		//if (g_Shop.CheckIsSelection())
		//{
		//	if (m_Input.isClick() == (int)GameInput_M::MouseValue::Left)
		//	{
		//		/*if (InCircle(850, 635, GameInput_M::Input::GetInstance().getMouseX()
		//			, GameInput_M::Input::GetInstance().getMouseY()))
		//			g_Gacha.GetGacha(TRUE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());
		//		else if (InCircle(1200, 635, g_MainGame.mx, g_MainGame.my))
		//			g_Gacha.GetGacha(FALSE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());*/

		//		m_State.ChangeState(GameState_M::E_InGameState::LuckyBox);
		//	}
		//	g_Gacha.DrawGachaButton(p_hdc, g_DeckBuild, g_Shop.GetSelectedChest(), g_MainGame.mx, g_MainGame.my, text);
		//}

		g_Shop.DrawShop(p_hdc, text);

		if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Right)
			m_State.ChangeState(GameState_M::E_InGameState::DeckBuild);
	}

	void Shop::Exit()
	{
		g_Shop.ExitShop();
	}

	void LuckyBox::Enter()
	{
	}

	void LuckyBox::Update(HDC p_hdc, HWND p_hwnd)
	{
		WCHAR text[256] = L"";
		if(m_Input.isClick())
			m_Input.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
		g_Gacha.InGacha();
		g_Gacha.DrawGacha(p_hdc, g_MainGame.mx, g_MainGame.my, text);
	}

	void LuckyBox::Exit()
	{
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