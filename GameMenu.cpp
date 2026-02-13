#include "GameMenu.h"
#include "RenderManager.h"
#include "InputGame.h"
#include "GameState.h"

void GameMenu::Enter()
{
	//Background
	RENDER.SetImage(L"background_menu.jpg", "Menu_Button_BackGround"
		, Rect(0, 0, 1360, 752), Rect(0, 0, 1280, 720)
		, true, GameImage_M::LayerType::Background);

	//전장 버튼
	int x, y;
	x = 330, y = 195;
	ButtonMenu.AddButton(make_shared<RectButton>("Stage", RECT{ x,y,x + 600,y + 75 }));
	//RENDER.SetImage(L"rect_button.png", "Stage_Button_Pos"
	//	, Rect(0, 0, 100, 100), Rect(x, y, 600, 75)
	//	, true, GameImage_M::LayerType::UI);

	//상점 버튼
	y += 125;
	ButtonMenu.AddButton(make_shared<RectButton>("Shop", RECT{ x,y,x + 600,y + 75 }));
	//RENDER.SetImage(L"rect_button.png", "Shop_Button_Pos"
	//	, Rect(0, 0, 100, 100), Rect(x, y, 600, 75)
	//	, true, GameImage_M::LayerType::UI);

	//로비로 돌아가기 버튼
	x += 55;
	y += 250;
	ButtonMenu.AddButton(make_shared<RectButton>("Lobby", RECT{ x,y,x + 485,y + 75 }));
	//RENDER.SetImage(L"rect_button.png", "Exit_Button_Pos"
	//	, Rect(0, 0, 100, 100), Rect(x, y, 485, 75)
	//	, true, GameImage_M::LayerType::UI);

}

void GameMenu::Logic()
{
	int mx, my;
	INPUT.GetMousePos(&mx, &my);

	if (ButtonMenu.HandleClickId(mx, my) == "Stage" 
		&& INPUT.isOneClick(GameInput_M::MouseValue::Left)) {
		STATE.ChangeState(GameState_M::E_InGameState::Stage);
	}
	if (ButtonMenu.HandleClickId(mx, my) == "Shop"
		&& INPUT.isOneClick(GameInput_M::MouseValue::Left)) {
		STATE.ChangeState(GameState_M::E_InGameState::Shop);
	}
	if (ButtonMenu.HandleClickId(mx, my) == "Lobby"
		&& INPUT.isOneClick(GameInput_M::MouseValue::Left)) {
		STATE.ChangeState(GameState_M::E_InGameState::Lobby);
	}
}

void GameMenu::Exit() {
	RENDER.RemoveIDIamage("Menu_Button_BackGround");
}
