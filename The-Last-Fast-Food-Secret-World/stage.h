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
	E_STAGE_TUTORIAL,
	E_STAGE_NORMAL,
	E_STAGE_BOSS,
	E_STAGE_LASTBOSS,
	E_STAGE_GAMECLEAR
};

enum StageName
{
	//hmmmmm
};



class Stage : public MainGame

{
protected:


public:
	int StageState; //스테이지 상태
	bool IsBossStage(); //보스 스테이지 확인
	bool IsLastBossStage(); //막보 스테이지 확인
	//bool GameStart(); //寃뚯엫 ?쒖옉
	bool IsGameOver(); //寃뚯엫 ?ㅻ쾭
	void IsStageClear(); //?ㅽ뀒?댁? ?대━??
	int IncreaseStage(); //?ㅼ쓬 ?ㅽ뀒?댁? 
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
		RENDER.SetImage(L"InGameResultBG_Win.png", "StageClear",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);
		RENDER.SetImage(L"InGameResultBG_Lost.png", "StageLost",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);
		RENDER.SetImage(L"InGameResultBG_BossWin.png", "Boss_StageClear",
			Rect(0, 0, 1280, 720), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::Background);

		RENDER.SetImage(L"StageClearBtn1.png", "StageClearBtn_1", //100x50
			Rect(0, 0, 96, 48), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		RENDER.SetImage(L"IngameResult_CardBox.png", "IngameResult_CardBox", //100x50
			Rect(0, 0, 256, 256), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		RENDER.SetImage(L"IngameResult_Coin.png", "IngameResult_Gold", //100x50
			Rect(0, 0, 256, 256), Rect(0, 0, 0, 0), false, GameImage_M::LayerType::UI);
		
		
	}
	void SetInGameResult();
	void DrawStageClearButton();
	void DrawInGameResult();
	void ExitInGameResult();
};

extern StageClear g_StageResult;
extern TutorialStage g_TutorialStage;
