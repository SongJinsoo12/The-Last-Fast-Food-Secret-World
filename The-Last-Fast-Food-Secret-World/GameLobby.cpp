#include "GameLobby.h"
#include "RenderManager.h"
#include "ButtonManager.h"
#include "InputGame.h"
#include "GameState.h"
#include "macroNum.h"

void GameLobby::Enter(){
	int x, y;
	
	x = 460, y = 575;

	//RENDER.SetImage(L"rect_button.png", "Start_Button_Pos"
	//	, Rect(0, 0, 100, 100), Rect(x, y, 380, 100)
	//	, true, GameImage_M::LayerType::UI);
	RENDER.SetImage(L"background_lobby.jpg", "Lobby_Button_BackGround"
		, Rect(0, 0, 1360, 752), Rect(0, 0, 1280, 720)
		, true, GameImage_M::LayerType::Background);

	btnManager.AddButton(make_shared<RectButton>("start", RECT{ x,y,x+380,y+100 }));
}

void GameLobby::Logic()
{
	int mx, my;
	INPUT.GetMousePos(&mx, &my);

	if (btnManager.HandleClickId(mx, my) == "start"
		&& INPUT.isOneClick(GameInput_M::MouseValue::Left)) {
		STATE.ChangeState(GameState_M::E_InGameState::Menu);
	}
}

void GameLobby::Exit()
{
	RENDER.RemoveIDIamage("Lobby_Button_BackGround");
}
