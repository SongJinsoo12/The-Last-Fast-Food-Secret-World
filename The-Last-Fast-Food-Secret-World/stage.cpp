
#include "Stage.h"
#include "RenderManager.h"
#include "MainGame.h"
#include "ButtonManager.h"

StageClear g_StageResult;
TutorialStage g_TutorialStage;








//bool Stage::GameStart() { 
//    
//    SmallStage = 1;
//    LargeStage = 1;
//    StageState = E_STAGE_TUTORIAL;
//    GameState = E_PVP;
//    return true;
//}
bool TutorialStage::CheckTutorial() {
    if (GetLargeStage() == 1 && GetSmallStage() <= 3)
    {
        StageState = E_STAGE_TUTORIAL;
        cout << "튜토리얼 속행" << endl;
        return IsTutorial = true;
    }
    else
    {
        StageState = E_STAGE_NORMAL;
        cout << "튜토리얼 종료" << endl;
        return IsTutorial = false;
    }

}

int Stage::DropReward() {
	
    if (SmallStage % 5 == 0)
    {
       
        cout << "골드, 카드박스 드롭됨" << endl;
        CardBox++; //카드박스10x 획득
        return LargeStage * SmallStage * 100; //보스

    }

    cout << "골드 드롭됨" << endl;
    return LargeStage * SmallStage * 50; // 잡몹


}


bool Stage::IsBossStage() {
    if (SmallStage == 5)
    {
        if (IsLastBossStage())
        {
            return true;
        }


        StageState = E_STAGE_BOSS;
        return true;
    }
    else
    {
        StageState = E_STAGE_NORMAL;
        return false;
    }

}

bool Stage::IsLastBossStage() {
    if (LargeStage == 6 && SmallStage == 5)
    {
        StageState = E_STAGE_LASTBOSS;
        return true;
    }
    return false;
} //라보 스테이지 체크

TCardType TutorialStage::TutorialCard() {
    
        if (GetLargeStage() == 1 && GetSmallStage() == 1)
            return E_TUTORIAL_ATTACK;

        if (GetLargeStage() == 1 && GetSmallStage() == 2)
            return E_TUTORIAL_DEFENSE;

        if (GetLargeStage() == 1 && GetSmallStage() == 3)
            return E_TUTORIAL_MAGIC;
    

    // 카드.h enum
}

//bool Stage::StageStart() {
//    return true;
//}




//enum State {
//    E_START,
//    E_MENU,
//    E_SHOP,
//    E_PVP,
//    E_STAGE_CLEAR,
//    E_DEAD,
//    E_GAMEOVER
//};
void Stage::IsStageClear() {
    

    if (g_MainGame.GameState == E_STAGE_CLEAR )
    {
        IsBossStage();
        //return true;
    }
    else
    {
        g_MainGame.GameState = E_GAMEOVER;
    }

    
   
        

    

    AddGold(DropReward());
	//cout << "골드, 카드획득>> 골드: " <<  <<" 카드: " << CardBox << endl;
}

int Stage::IncreaseStage() {
    
    if (g_MainGame.GameState == E_STAGE_CLEAR) {
        if (SmallStage != 5)
        {
            SmallStage++;
        }
        else
        {
            LargeStage++;
            SmallStage = 1;
        }
       
    }
    
    if (g_TutorialStage.CheckTutorial())
    {
        g_TutorialStage.TutorialCard();
    }
    IsBossStage();

    return GameState;
}

//bool Stage::IsGameClear() {
//    return StageState == E_STAGE_GAMECLEAR;
//}

bool Stage::IsGameOver() {

    if (g_MainGame.GameState != E_DEAD) {

        return false;
    }
    else
    {
        g_MainGame.GameState = E_GAMEOVER;

        return true;
    }
}
//enum State {
//    E_START,
//    E_MENU,
//    E_SHOP,
//    E_PVP,
//    E_STAGE_CLEAR,
//    E_DEAD,
//    E_GAMEOVER
//};


void StageClear::DrawStageClearButton() {
    if (g_MainGame.GameState == E_STAGE_CLEAR) {
        m_StageClearBtn.AddButton(make_shared<RectButton>("StageClearBtn_1", RECT{ 640 - 96, 360 - 48, 640 - 96 + 144, 360 - 48 + 70 }));
    }
    else if (g_MainGame.GameState == E_GAMEOVER) {
        m_StageClearBtn.AddButton(make_shared<RectButton>("StageLostBtn_1", RECT{ 640 - 96, 360 - 48, 640 - 96 + 144, 360 - 48 + 70 }));
    } 

}


void StageClear :: SetInGameResult() {
    RENDER.MoveImage("StageClear",
        Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("StageLost",
        Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("Boss_StageClear",
		Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("StageClearBtn_1",
        Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
	RENDER.MoveImage("StageLostBtn_1",
		Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
    RENDER.MoveImage("InGameResult_Gold",
        Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
    RENDER.MoveImage("InGameResult_CardBox",
        Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
}


void StageClear::DrawInGameResult() {
    switch (g_MainGame.GameState) {
    case E_STAGE_CLEAR:
        if (StageState == E_STAGE_NORMAL) {
            RENDER.ImageVisible("StageClear", true);//배경
            RENDER.ImageVisible("StageClearBtn_1", true);//클리어버튼 보이기
            RENDER.ImageVisible("IngameResult_Gold", true); //골드
        }
        if (StageState == E_STAGE_BOSS) {
            RENDER.ImageVisible("IngameResult_CardBox", true);//카드박스 보이기
        }
        break;
    case E_GAMEOVER:
        RENDER.ImageVisible("StageLost", true);//게임오버 이미지 보이기
		RENDER.ImageVisible("StageLostBtn_1", true);//게임오버 버튼 보이기
		break;
    }           
		
        RENDER.MoveImage("StageClear",
                    Gdiplus::Rect(0, 0, 1280, 720));
        RENDER.MoveImage("StageClearBtn_1",
                    Gdiplus::Rect(640-96, 360-48, 144, 70));//크기조절 //96 48

        /*RENDER.MoveImage("StageClearBtn_1",
            Gdiplus::Rect(650, 360, 1950, 1080));*/
        
}

void StageClear::ExitInGameResult() {
    
        RENDER.ImageVisible("StageClear", false);
        RENDER.ImageVisible("GameOver", false);

	    RENDER.ImageVisible("StageClearBtn_1", false);
        RENDER.ImageVisible("StageLostBtn_1", false);

        RENDER.ImageVisible("IngameResult_Gold", false); 
		RENDER.ImageVisible("IngameResult_CardBox", false);
        g_MainGame.GameState = E_MENU;

	cout << "InGameResult Exit" << endl;
}


