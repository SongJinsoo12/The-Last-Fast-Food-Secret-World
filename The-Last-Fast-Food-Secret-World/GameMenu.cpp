#include "GameMenu.h"
#include "RenderManager.h"
#include "ButtonManager.h"
#include "InputGame.h"
#include "GameState.h"

void GameMenu::Enter()
{
	RENDER.SetImage(L"background_menu.jpg", "Lobby_Button_BackGround"
		, Rect(0, 0, 1360, 752), Rect(0, 0, 1280, 720)
		, true, GameImage_M::LayerType::Background);
}

void GameMenu::Logic()
{

}

void GameMenu::Exit() {
	RENDER.RemoveIDIamage("Lobby_Button_BackGround");
}
