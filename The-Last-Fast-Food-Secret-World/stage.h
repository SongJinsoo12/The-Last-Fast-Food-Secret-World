#pragma once
#include <iostream>
#include "MainGame.h"
#include "InputGame.h"
#include "RenderManager.h"
#include "ImageManager.h"
#include "ButtonManager.h"


using namespace std;

enum StageState
{
	E_STAGE_TUTORIAL, //튜토리얼
	E_STAGE_NORMAL,
	E_STAGE_BOSS,
	E_STAGE_LASTBOSS,
	E_STAGE_GAMECLEAR
};

enum StageName
{
	//hmmmmm
};



class Stage

{
private:
	int L_Stage;
	int S_Stage;
	

public:
	Stage();
	int StageState; //스테이지 상태
	bool IsBossStage(); //보스 스테이지 확인
	bool IsLastBossStage(); //막보 스테이지 확인

	//bool GameStart(); //寃뚯엫 ?쒖옉
	bool IsGameOver(); //寃뚯엫 ?ㅻ쾭

	
	void IsStageClear(); //?ㅽ뀒?댁? ?대━??

	//bool IsGameClear(); //寃뚯엫 ?대━???뺤씤
	void IncreaseStage(); //?ㅼ쓬 ?ㅽ뀒?댁? 

	int DropReward(); //?ㅽ뀒?댁????곕씪 怨⑤뱶 ?ㅼ젙
	
		


};

enum TCardType
{
	E_TUTORIAL_ATTACK,
	E_TUTORIAL_DEFENSE,
	E_TUTORIAL_MAGIC
};

class TutorialStage : public Stage
{
private:
	bool IsTutorial;

public:

	TCardType TutorialCard(); //?쒗넗由ъ뼹 ?ㅽ뀒?댁???留욌뒗 移대뱶 諛섑솚
	bool CheckTutorial(); //?쒗넗由ъ뼹 ?ㅽ뀒?댁? ?뺤씤
};

//extern TutorialStage g_TutorialStage;

class StageClear : public Stage
{
public:
	ButtonManager m_StageClearBtn;
public:
	StageClear()
	{
		RENDER.SetImage(L"InGameResult_BG_Win.png", "InGameResult_BG_StageClear",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);
		RENDER.SetImage(L"InGameResult_BG_Lost.png", "InGameResult_BG_StageLost",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);
		RENDER.SetImage(L"InGameResult_BG_BossWin.png", "InGameResult_BG_Boss_StageClear",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);

		RENDER.SetImage(L"InGameResult_UI_StageClearBtn.png", "InGameResult_UI_StageClearBtn1", //100x50
			Rect(0, 0, 96, 48), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		RENDER.SetImage(L"IngameResult_UI_CardBox.png", "InGameResult_UI_CardBox", //100x50
			Rect(0, 0, 256, 256), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		RENDER.SetImage(L"IngameResult_UI_Coin.png", "InGameResult_UI_Gold", //100x50
			Rect(0, 0, 256, 256), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		
		
	}
	void SetInGameResult();
	void DrawStageClearButton();
	void DrawInGameResult();
	void ExitInGameResult();
};

extern StageClear g_StageResult;
extern TutorialStage g_TutorialStage;
