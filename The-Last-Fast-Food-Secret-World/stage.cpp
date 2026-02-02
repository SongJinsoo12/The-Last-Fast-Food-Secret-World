#include "stage.h"
#include "RenderManager.h"
#include "MainGame.h"
#include "ButtonManager.h"






int Stage::Get_LargeStage() { //튜토리얼 클래스 용
    return LargeStage;
}

int Stage::Get_SmallStage() { //튜토리얼 클래스 용
    return SmallStage;
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
    if (Get_LargeStage() <= 1 && Get_SmallStage() <= 3)
    {
        StageState = E_STAGE_TUTORIAL;
        return IsTutorial = true;
        cout << "튜토리얼 속행" << endl;
    }
    else
    {
        StageState = E_STAGE_NORMAL;
        return IsTutorial = false;
        cout << "튜토리얼 종료" << endl;
    }
    
}

int Stage::DropReward() {
    
    if (SmallStage % 5 == 0)
    {
        //팩 뽑기 횟수 변수?
        cout << "골드, 카드박스 드롭됨" << endl;
        return LargeStage * SmallStage * 100; //보스

    }

    return LargeStage * SmallStage * 50; // 잡몹
    cout << "골드 드롭됨" << endl;
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
    if (CheckTutorial()) {

        if (Get_LargeStage() == 1 && Get_SmallStage() == 1)
            return E_TUTORIAL_ATTACK;

        if (Get_LargeStage() == 1 && Get_SmallStage() == 2)
            return E_TUTORIAL_DEFENSE;

        if (Get_LargeStage() == 1 && Get_SmallStage() == 3)
            return E_TUTORIAL_MAGIC;
    }

    // 카드.h enum
}

bool Stage::StageStart() {
    return true;
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
int Stage::StageClear(bool isMonsterLost) {

    if (IsLastBossStage())
        StageState = E_STAGE_GAMECLEAR;


    if (!isMonsterLost) //몬스터 hp = 0 또는 몬스터 비활성화일떄 else 코드 실행
    {
        
        GameState = E_PVP;
        return GameState;
    }
    else
    {
        GameState = E_STAGE_CLEAR;

    }

    AddGold(DropReward());
    
    
    
    //GameState = E_MENU;
    return GameState;

}

int Stage::NextStage() {
    
    if (GameState == E_MENU) {
        if (SmallStage != 5)
        {
            SmallStage++;
        }
        else
        {
            LargeStage++;
            SmallStage = 1;
        }
        GameState = E_PVP;
    }
    IsBossStage();
    return GameState;
}

bool Stage::IsGameClear() {
    return StageState == E_STAGE_GAMECLEAR;
}

bool Stage::GameOver(bool isPlayerLost/* player.h  IsAlive()*/) {

    if (!isPlayerLost) {

        GameState = E_PVP;
        return false;
    }
    else
    {
        GameState = E_GAMEOVER;

        return true;
    }
}

void Stage::SetImage() {
    m_rend.SetImage(L"StageClear.png", "StageClear",
        Rect(0, 0, 1024, 1024), Rect(0, 0, 1280, 720), true, GameImage_M::LayerType::UI);
    m_rend.SetImage(L"StageClearBnt_1.png", "StageClearBtn_1",
        Rect(0, 0, 1200, 1200), Rect(0, 0, 1280, 720), true, GameImage_M::LayerType::UI);
    cout << "스테이지 클리어 화면 로드 확인\n";

}

void Stage::LoadStageClearScreen() {

    m_rend.MoveImage("StageClear",
        Gdiplus::Rect(0, 0, 500, 500));
    btnManager.AddButton(make_shared<RectButton>( "StageClearBtn_1", Rect(390, 500, 500, 100), true));
	cout << "스테이지 클리어 화면 출력 확인" << endl;
    //while?
    if (btnManager.HandleClick(mg.mx, mg.my) && btnManager.HandleClick(mg.mx, mg.my) -> GetId() == "StageClearBtn_1")
    {
        GameState = E_MENU;
    }
    
}

