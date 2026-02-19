
#include "Stage.h"
#include "RenderManager.h"
#include "MainGame.h"
#include "ButtonManager.h"

StageClear g_StageResult;
TutorialStage g_TutorialStage;


Stage::Stage() {
    L_Stage = 1;
    S_Stage = 1;
    StageState = E_STAGE_NORMAL;

}





//bool Stage::GameStart() { 
//    
//    SmallStage = 1;
//    LargeStage = 1;
//    StageState = E_STAGE_TUTORIAL;
//    GameState = E_PVP;
//    return true;
//}
bool TutorialStage::CheckTutorial() {
    if (g_MainGame.GetLargeStage() == 1 && g_MainGame.GetSmallStage() <= 3)
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

//int Stage::DropReward() {
//	
//    if (S_Stage % 5 == 0)
//    {
//       
//        cout << "골드, 카드박스 드롭됨" << endl;
//        P_CardBox++; //카드박스10x 획득
//        return L_Stage * S_Stage * 100; //보스
//
//    }
//
//    cout << "골드 드롭됨" << endl;
//    return L_Stage * S_Stage * 50; // 잡몹
//
//
//}
// 테스트
int Stage::DropReward() {

    if (StageState == E_STAGE_BOSS)
    {

        cout << "골드, 카드박스 드롭됨" << endl;
        g_MainGame.AddCardBox(1); //카드박스10x 획득
        return L_Stage * S_Stage * 100; //보스

    }

    cout << "골드 드롭됨" << endl;
    return L_Stage * S_Stage * 50; // 잡몹


}


bool Stage::IsBossStage() {
    if (S_Stage == 5)
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
    if (L_Stage == 6 && S_Stage == 5)
    {
        StageState = E_STAGE_LASTBOSS;
        return true;
    }
    return false;
} //라보 스테이지 체크

TCardType TutorialStage::TutorialCard() {
    
        if (g_MainGame.GetLargeStage() == 1 && g_MainGame.GetSmallStage() == 1)
            return E_TUTORIAL_ATTACK;

        if (g_MainGame.GetLargeStage() == 1 && g_MainGame.GetSmallStage() == 2)
            return E_TUTORIAL_DEFENSE;

        if (g_MainGame.GetLargeStage() == 1 && g_MainGame.GetSmallStage() == 3)
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
        //IsBossStage();
        g_MainGame.AddGold(DropReward());
        //return true;
    }
    else
    {
        g_MainGame.GameState = E_GAMEOVER;
    }

    
   
        

    

    
	//cout << "골드, 카드획득>> 골드: " <<  <<" 카드: " << CardBox << endl;
}

void Stage::IncreaseStage() {
    


    if (g_MainGame.GameState == E_STAGE_CLEAR && StageState != E_STAGE_GAMECLEAR) {
        if (S_Stage != 5)
        {
            S_Stage++;
        }
        else
        {
            L_Stage++;
            S_Stage = 1;
        }
       
    }

    g_MainGame.AddStage(L_Stage, S_Stage);

    if (g_TutorialStage.CheckTutorial())
    {
        g_TutorialStage.TutorialCard();
    }
    IsBossStage();

    
   
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
        m_StageClearBtn.AddButton(make_shared<RectButton>("InGameResult_UI_StageClearBtn1", RECT{ 640 - 96, 360 - 48, 640 - 96 + 144, 360 - 48 + 70 }));
    }
    else if (g_MainGame.GameState == E_GAMEOVER) {
        m_StageClearBtn.AddButton(make_shared<RectButton>("InGameResult_UI_StageLostBtn1", RECT{ 640 - 96, 360 - 48, 640 - 96 + 144, 360 - 48 + 70 }));
    } 

}


void StageClear :: SetInGameResult() {
    RENDER.MoveImage("InGameResult_BG_StageClear",
        Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("InGame_Result_BG_StageLost",
        Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("InGameResult_BG_BossStageClear",
		Gdiplus::Rect(0, 0, 1280, 720));
    RENDER.MoveImage("InGameResult_UI_StageClearBtn1",
        Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
	RENDER.MoveImage("InGameResult_UI_StageLostBtn1",
		Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
    RENDER.MoveImage("InGameResult_UI_Gold",
        Gdiplus::Rect(300 - 96, 50, 144, 70));//크기조절 //96 48
    RENDER.MoveImage("InGameResult_UI_CardBox",
        Gdiplus::Rect(640 - 96, 360 - 48, 144, 70));//크기조절 //96 48
}


void StageClear::DrawInGameResult() {
    switch (g_MainGame.GameState) {
    case E_STAGE_CLEAR:
        if (StageState == E_STAGE_TUTORIAL) {
            RENDER.ImageVisible("InGameResult_BG_StageClear", true);//배경
            RENDER.ImageVisible("InGameResult_UI_StageClearBtn1", true);//클리어버튼 보이기
            RENDER.ImageVisible("InGameResult_UI_Gold", true); //골드
        }
        if (StageState == E_STAGE_NORMAL) {
            RENDER.ImageVisible("InGameResult_BG_StageClear", true);//배경
            RENDER.ImageVisible("InGameResult_UI_StageClearBtn1", true);//클리어버튼 보이기
            RENDER.ImageVisible("InGameResult_UI_Gold", true); //골드
        }
        if (StageState == E_STAGE_BOSS) {
            RENDER.ImageVisible("InGameResult_BG_StageClear", true);//배경
            RENDER.ImageVisible("InGameResult_UI_StageClearBtn_1", true);//
            RENDER.ImageVisible("InGameResult_UI_Gold", true);
            RENDER.ImageVisible("InGameResult_UI_CardBox", true);//카드박스 보이기
        }
            if (StageState == E_STAGE_LASTBOSS) {

        
        }
        break;
    case E_GAMEOVER:
        RENDER.ImageVisible("InGameResult_UI_StageLost", true);//게임오버 이미지 보이기
		RENDER.ImageVisible("InGameResult_UI_StageLostBtn1", true);//게임오버 버튼 보이기
		break;
    }           
		
        //RENDER.MoveImage("InGameResult_StageClear",
        //            Gdiplus::Rect(0, 0, 1280, 720));
        //RENDER.MoveImage("InGameResult_StageClearBtn_1",
        //            Gdiplus::Rect(640-96, 360-48, 144, 70));//크기조절 //96 48

        /*RENDER.MoveImage("StageClearBtn_1",
            Gdiplus::Rect(650, 360, 1950, 1080));*/
        
}

void StageClear::ExitInGameResult() {
    
        RENDER.ImageVisible("InGameResult_BG_StageClear", false);
        RENDER.ImageVisible("InGameResult_BG_GameOver", false);

	    RENDER.ImageVisible("InGameResult_UI_StageClearBtn1", false);
        RENDER.ImageVisible("InGameResult_UI_StageLostBtn1", false);

        RENDER.ImageVisible("InGameResult_UI_Gold", false); 
		RENDER.ImageVisible("InGameResult_UI_CardBox", false);
        g_MainGame.GameState = E_MENU;
		//IncreaseStage();
	cout << "InGameResult Exit" << endl;
}


