#include "GameState.h"
#include "RenderManager.h"
#include "InputGame.h"
#include <iostream>

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

	// �� ȭ�鿡 �´� Update ����
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
		GameInput_M::Input::GetInstance().isClick();
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {
		
	}

	void Lobby::Exit() {
		
	}

	void Menu::Enter()
	{
	
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{

	}

	void Menu::Exit() {
		
	}

	void DeckBuild::Enter() {

	}

	void DeckBuild::Update(HDC p_hdc, HWND p_hwnd) {

	}

	void DeckBuild::Exit() {

	}

	void Shop::Enter()
	{
		//g_Shop.SetDrawShop();
	}

	void Shop::Update(HDC p_hdc, HWND p_hwnd)
	{
		//if (g_Input.isClick() == (int)GameInput_M::MouseValue::Left)
		//	g_State.ChangeState(GameState_M::E_InGameState::Lobby);
		//g_Shop.DrawShop(p_hdc, text);
	}

	void Shop::Exit()
	{
		//g_Shop.ClearShop();
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
