#include "GameState.h"
#include "RenderManager.h"
#include "InputGame.h"
#include "CardTableManager.h"
#include "Sound.h"
#include <chrono>

//각 화면
#include "GameLobby.h"
#include "GameMenu.h" 
#include "Shop.h"
#include "DeckBuilding.h"
#include "CardGacha.h"
#include "Stage.h"
#include "MainGame.h"
#include "CardManager.h"

extern Player g_playerActor;
extern Mob g_enemyActor;

namespace GameState_M {
	Context::Context() {
		StateVector.resize((int)E_InGameState::InGameResult + 1);
		StateVector[(int)E_InGameState::Lobby] = make_shared<Lobby>();
		StateVector[(int)E_InGameState::Menu] = make_shared<Menu>();
		StateVector[(int)E_InGameState::Stage] = make_shared<Stage>();
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

	void Context::Init()
	{
		//시작시 덱 로드, 세이브는 소멸자에
		g_DeckBuild.LoadDeck(g_Gacha.GetAllCard());
		g_MainGame.AddGold(1000);
	}

	void Lobby::Enter()
	{
		GameLobby::GetInstance().Enter();
	}

	void Lobby::Update(HDC p_hdc, HWND p_hwn) {
		GameLobby::GetInstance().Logic();
	}

	void Lobby::Exit() {
		GameLobby::GetInstance().Exit();
	}

	void Menu::Enter()
	{
		GameMenu::GetInstance().Enter();
	}

	void Menu::Update(HDC p_hdc, HWND p_hwnd)
	{
		GameMenu::GetInstance().Logic();
	}

	void Menu::Exit() {
		GameMenu::GetInstance().Exit();
	}

	void DeckBuild::Enter() {
		//덱빌드진입
		g_DeckBuild.EnterDeckBuild();
	}

	void DeckBuild::Update(HDC p_hdc, HWND p_hwnd) {
		WCHAR text[256] = L"";

		//화면 그리기
		g_DeckBuild.DrawDeckBuild(p_hdc, text);

		//좌클릭시 카드 선택
		if (INPUT.isClick() == (int)GameInput_M::MouseValue::Left)
		{
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			if (btnManager.HandleClickId(g_MainGame.mx, g_MainGame.my) == "rb1")
			{
				cout << "버튼클릭" << endl;
				STATE.ChangeState(GameState_M::E_InGameState::Shop);
			}
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'L');
		}
		//우클릭시 카드 이동
		else if (INPUT.isClick() == (int)GameInput_M::MouseValue::Right)
		{
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
			g_DeckBuild.DeckBuild(g_MainGame.mx, g_MainGame.my, 'R');
		}

	}

	void DeckBuild::Exit() {
		//덱빌드 퇴장
		g_DeckBuild.ExitDeckBuild();
	}

	void Shop::Enter()
	{
		//상점 진입
		g_Shop.SetDrawShop();
		cout << "Shop Enter" << endl;
	}

	void Shop::Update(HDC p_hdc, HWND p_hwnd)
	{
		WCHAR text[256] = L"";

		if (g_Shop.DrawEnterShop())
		{
			//상점 화면 그리기
			g_Shop.DrawShop(p_hdc, text);

			//좌클릭시 상자선택
			if (INPUT.isClick() == (int)GameInput_M::MouseValue::Left)
			{
				INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
				if (g_Shop.m_ShopBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "back")
				{
					STATE.ChangeState(GameState_M::E_InGameState::Menu);
				}
				g_Shop.SelectChest(g_MainGame.mx, g_MainGame.my);
			}

			//상자가 선택된 경우 -> 뽑기버튼 활성화 및 뽑기화면전환
			if (g_Shop.CheckIsSelection())
			{
				if (g_Shop.m_ShopBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "one" && g_Shop.isSucceedGacha(p_hdc, 1))
				{
					g_Gacha.GetGacha(TRUE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());
					STATE.ChangeState(GameState_M::E_InGameState::LuckyBox);
				}
				else if (g_Shop.m_ShopBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "ten" && g_Shop.isSucceedGacha(p_hdc, 10))
				{
					g_Gacha.GetGacha(FALSE, g_DeckBuild, g_MainGame, g_Shop.GetSelectedChest());
					STATE.ChangeState(GameState_M::E_InGameState::LuckyBox);
				}
				g_Shop.DrawGachaButton(p_hdc, g_MainGame.mx, g_MainGame.my, text);
			}

			//임시 - 우클릭 시 덱빌딩화면 전환
			if (GameInput_M::Input::GetInstance().isClick() == (int)GameInput_M::MouseValue::Right)
				STATE.ChangeState(GameState_M::E_InGameState::DeckBuild);
		}
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
		if (g_Gacha.m_GachaBtnM.HandleClickId(g_MainGame.mx, g_MainGame.my) == "back")
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
		m_player.BindActors(&g_playerActor, &g_enemyActor);
		m_boss.BindActors(nullptr, &g_enemyActor);
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

		//�ִϸ��̼� �۾�
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

		//bgm
		Sound::Instance()->PlayBgm();
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

			//����Ʈ ��
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
		// ✅ 현재 턴 주인만 타임리밋(턴 전환)을 돌린다
		if (m_player.IsMyTurn())
			m_player.TimeLimit(m_player, m_boss);
		else
			m_boss.TimeLimit(m_player, m_boss);

		// ✅ 내 턴일 때만 카드 입력(사용) 처리
		if (m_player.IsMyTurn())
			m_player.HandSelect(m_player, m_boss);
		//m_player.TimeLimit(m_player, m_boss);
		////게임 로직 함수
		//m_player.HandSelect(m_player, m_boss);

		//패 출력
		m_player.DrawPlayerHand();
		m_boss.DrawOppHand();


		PlayAnimation(m_shiny, m_shinyEffect, m_player.GetIsShiny());
		PlayAnimation(m_rip, m_ripEffect, m_player.GetIsRip());
		PlayAnimation(m_skill, m_skillEffect, m_player.GetIsSkill());
		PlayAnimation(m_def, m_defEffect, m_player.GetIsDef());



		/*MonoGram("Song123", "Temp_", 0, 0);
		MonoGram("Song123", "Hello_", 0, 100);*/
	}

	void InGame::Exit()
	{
		//이미지 지우기
	}

	void InGameResult::Enter()
	{
		g_StageResult.DrawInGameResult();
		g_StageResult.DrawStageClearButton();
		cout << "InGameResult Enter" << endl;
	}

	void InGameResult::Update(HDC p_hdc, HWND p_hwnd)
	{
		if (INPUT.isOneClick(GameInput_M::MouseValue::Left))
			INPUT.GetMousePos(&g_MainGame.mx, &g_MainGame.my);
		if (g_StageResult.m_StageClearBtn.HandleClickId(g_MainGame.mx, g_MainGame.my) == "StageClearBtn_1")
		{
			cout << "버튼 클릭됨" << endl;
			g_StageResult.NextStage();
			STATE.ChangeState(GameState_M::E_InGameState::Lobby);
			return;
		}
	}
	void InGameResult::Exit()
	{
		g_StageResult.ExitInGameResult();
	}


	void Stage::Enter()
	{
	}

	void Stage::Update(HDC p_hdc, HWND p_hwnd)
	{
	}

	void Stage::Exit()
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
