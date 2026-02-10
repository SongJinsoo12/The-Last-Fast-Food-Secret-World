#include "GameState.h"
#include "RenderManager.h"
#include "InputGame.h"
#include "CardTableManager.h"

//각 화면
//#include "Shop.h"

namespace GameState_M {
	Context::Context() {
		StateVector.resize((int)E_InGameState::InGameResult + 1);
		StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		StateVector[(int)E_InGameState::Menu] = make_shared<Menu>();
		StateVector[(int)E_InGameState::DeckBuild] = make_shared<DeckBuild>();
		StateVector[(int)E_InGameState::Shop] = make_shared<Shop>();
		StateVector[(int)E_InGameState::LuckyBox] = make_shared<LuckyBox>();
		StateVector[(int)E_InGameState::InGame] = make_shared<InGame>();
		StateVector[(int)E_InGameState::InGameResult] = make_shared<InGameResult>();
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
		/*M_REND.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(0, 0, 300, 300)
			, false, GameImage_M::LayerType::Field);*/
		
		/*int x = 0;
		int y = 12;
		for (size_t i = 0; i < 80; i++)
		{
			if (i > 0) x += 6;

			if (i % 16 == 0 && i != 0)
			{
				x = 0;
				y += 12;
			}
			string monoId = "Mono_";
			monoId += to_string(i);
			M_REND.SetImage(L"images/monogram-bitmap.png", monoId, Rect(x, y, 6, 12), Rect(0, 0, 0, 0)
				, false, GameImage_M::LayerType::UI);
		}*/
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {
		if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Left)
		{
			//m_State.ChangeState(GameState_M::E_InGameState::InGameResult);
			//M_REND.ImageVisible("ID_1", true);
			M_STATE.ChangeState(GameState_M::E_InGameState::InGame);
		}
	}

	void Lobby::Exit() {
		//M_REND.AllRemoveImage();
	}

	void Menu::Enter()
	{
		//M_REND.SetImage(L"test.jpg", "ID_1", Rect(0, 0, 512, 512), Rect(100, 0, 300, 300), true, GameImage_M::LayerType::Field);
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{

	}

	void Menu::Exit() {
		//M_REND.RemoveIDIamage("ID_1");
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
		//if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Left)
		//	m_State.ChangeState(GameState_M::E_InGameState::Lobby);
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
		//이미지 로드/보여기
		//값 초기화 가능 
		m_player.SetImage();
		m_player.DrawBG();
		CardTableManager::Instance();

		m_player.SetDeck();
		m_boss.SetDeck();
		m_player.StartTurn(m_player, m_boss);
		m_player.DrawPlayerHand();
		m_boss.DrawOppHand();

		//애니메이션 작업
		m_shinyEffect.SetId("Card_Shiny_");
		m_shinyEffect.SetImageSize(200, 200, 100, 128);
		m_shinyEffect.SetIndex(10);

		m_ripEffect.SetId("Card_Rip_");
		m_ripEffect.SetImageSize(300, 200, 100, 128);
		m_ripEffect.SetIndex(15);

		m_skillEffect.SetId("Card_Skill_");
		m_skillEffect.SetImageSize(430, 200, 128, 128);
		m_skillEffect.SetIndex(27);

		m_defEffect.SetId("Def_Effect_");
		m_defEffect.SetImageSize(600, 200, 192, 192);
		m_defEffect.SetIndex(20);
	}

	void InGame::PlayAnimation(Timer& p_timer, Animation& p_effect, bool* p_isPlay)
	{
		if (!*p_isPlay) return;

		if (!p_timer.GetIsStart())
		{
			p_timer.StartTimer();
			p_timer.SetIsStart(true);
		}

		string cardId = p_effect.GetId();
		cardId += to_string(p_timer.GetIndex());

		M_REND.MoveImage(cardId, Gdiplus::Rect(p_effect.GetImageX(), p_effect.GetImageY(),
			p_effect.GetImageWidth(), p_effect.GetImageHeight()));
		M_REND.ImageVisible(cardId, true);

		p_timer.UpdateTimer();
		if (p_timer.CheckTimer(0.05))
		{
			M_REND.ImageVisible(cardId, false);

			//이펙트 끝
			if (p_timer.GetIndex() >= p_effect.GetIndex())
			{
				p_timer.SetIndex(0);
				*p_isPlay = false;
				return;
			}

			p_timer.StartTimer();
			p_timer.PlusIndex();
			string newId = p_effect.GetId();
			newId += to_string(p_timer.GetIndex());
			M_REND.MoveImage(newId, Gdiplus::Rect(p_effect.GetImageX(), p_effect.GetImageY(),
				p_effect.GetImageWidth(), p_effect.GetImageHeight()));
			M_REND.ImageVisible(newId, true);
		}
	}


	void InGame::Update(HDC p_hdc, HWND p_hwnd)
	{
		m_player.TimeLimit(m_player, m_boss);
		//게임 로직 함수
		m_player.HandSelect(m_player, m_boss);

		//패 출력
		m_player.DrawPlayerHand();
		m_boss.DrawOppHand();


		PlayAnimation(m_shiny, m_shinyEffect, m_player.GetIsShiny());
		PlayAnimation(m_rip, m_ripEffect, m_player.GetIsRip());
		PlayAnimation(m_skill, m_skillEffect, m_player.GetIsSkill());
		PlayAnimation(m_def, m_defEffect, m_player.GetIsDef());



		MonoGram("Song123", "Temp_", 0, 0);
		MonoGram("Song123", "Hello_", 0, 100);
	}

	void InGame::Exit()
	{
		//이미지 지우기
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

	void State::MonoGram(string p_image, string p_id, int p_x, int p_y)
	{
		int tempX = p_x;
		for (size_t i = 0; i < p_image.size(); i++)
		{
			if (i > 0) tempX += 12;

			int imageX, imageY;
			int temp;

			temp = p_image[i];
			temp -= 32;

			string tempId = p_id;
			tempId += to_string(i);

			imageX = ((temp % 16) * 6);
			imageY = ((temp / 16) * 12);

			M_REND.SetImage(L"images/monogram-bitmap.png", tempId, Rect(imageX, imageY, 6, 12), Rect(tempX, p_y, 12, 24)
				, true, GameImage_M::LayerType::UI);
		}
	}

}