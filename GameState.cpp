#include "GameState.h"
#include "RenderManager.h"
#include "InputGame.h"

//각 화면
#include "Shop.h"

GameState_M::Context g_State;

namespace GameState_M {
	Context::Context() {
		StateVector.resize((int)E_InGameState::InGameResult + 1);
		StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		StateVector[(int)E_InGameState::Menu] = make_shared<Menu>();
		StateVector[(int)E_InGameState::DeckBuild] = make_shared<DeckBuild>();
		StateVector[(int)E_InGameState::Shop] = make_shared<Shop>();
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
		g_renderManager.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(0, 0, 300, 300), false);
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {
		if (g_Input.isClick() == (int)GameInput_M::MouseValue::Left)
			g_renderManager.ImageVisible("ID_1", true);
	}

	void Lobby::Exit() {
		g_renderManager.AllRemoveImage();
	}

	void Menu::Enter()
	{
		g_renderManager.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(100, 0, 300, 300), true);
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{

	}

	void Menu::Exit() {
		g_renderManager.RemoveIDIamage("ID_1");
	}

	void DeckBuild::Enter() {

	}

	void DeckBuild::Update(HDC p_hdc, HWND p_hwnd) {

	}

	void DeckBuild::Exit() {

	}

	void Shop::Enter()
	{
		g_Shop.SetDrawShop();
	}

	void Shop::Update(HDC p_hdc, HWND p_hwnd)
	{
		if (g_Input.isClick() == (int)GameInput_M::MouseValue::Left)
			g_State.ChangeState(GameState_M::E_InGameState::Lobby);
		g_Shop.DrawShop(p_hdc, text);
	}

	void Shop::Exit()
	{
		g_Shop.ClearShop();
	}

	void LuckyBox::Enter()
	{
	}

	void LuckyBox::Update(HDC p_hdc, HWND p_hwnd)
	{
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