#include "GameLobby.h"
#include "RenderManager.h"
#include "ButtonManager.h"
#include "InputGame.h"
#include "GameState.h"
#include "macroNum.h"

void GameLobby::Enter(){

	int x, y;
	//x = (rect.right - rect.left) / 2;
	//y = (rect.bottom - rect.top) / 2 + 100;

	x = (1280 - 100) / 2-50;
	y = (720-0) / 2 + 250;

	RENDER.SetImage(L"background_lobby.jpg", "Lobby_Button_BackGround"
		, Rect(0, 0, 1360, 752), Rect(0, 0, 1280, 720)
		, true, GameImage_M::LayerType::Background);

	btnManager.AddButton(make_shared<RectButton>("start", RECT{ x,y,x+200,y+50 }));
}

void GameLobby::Logic()
{
	int mx, my;
	INPUT.GetMousePos(&mx, &my);

	if (btnManager.HandleClickId(mx, my) == "start") {
		STATE.ChangeState(GameState_M::E_InGameState::Menu);
	}
}

void GameLobby::Exit()
{
	RENDER.RemoveIDIamage("Lobby_Button_BackGround");
}
