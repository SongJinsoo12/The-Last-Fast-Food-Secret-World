#include "stage.h"

#include "MainGame.h"






int Stage::Get_LargeStage() { //튜토리얼 클래스 용
    return LargeStage;
}

int Stage::Get_SmallStage() { //튜토리얼 클래스 용
    return SmallStage;
}


 

bool Stage::GameStart() { //수정중
    //GetStage();
    SmallStage = 1;
    LargeStage = 1;
    StageState = E_STAGE_TUTORIAL;
    GameState = E_PVP;
    return true;
}
bool TutorialStage::CheckTutorial() {
    if ( Get_LargeStage() <= 3 && Get_SmallStage() <= 3)
    {
        StageState = E_STAGE_TUTORIAL;
        return IsTutorial = true;
    }
    else
    {
        StageState = E_STAGE_NORMAL;
        return IsTutorial = false;
    }
}

int Stage::setgold() {
    if (SmallStage % 5 == 0)
        return LargeStage * SmallStage * 100; //보스
    return LargeStage * SmallStage * 50; // 잡몹
}

//
//{
// 카드박스 드롭 함수?
//}

bool Stage::IsBossStage() {
    if (SmallStage == 5)
    {
        if (IsLastBossStage())
            return true;
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

    AddGold(setgold());
    DropCard();
    //if (
    //보상 화면출력 함수?
    //GameState = E_MENU;
    return GameState; 

}

int Stage::NextStage() {
    /*if (IsGameClear() == 7)
        return false;*/

    if (StageClear(/* ? */) == E_MENU) {
        
            

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
        return GameState;
    }
}

bool Stage::IsGameClear() {
    if (StageClear(/* ? */) == E_STAGE_GAMECLEAR) {
        return true;
    }
}

bool Stage::GameOver(bool isPlayerLost) {

    if (!isPlayerLost) { //플레이어 hp = 0
        GameState = E_PVP;
        return false;
    }
    else
    {
        GameState = E_GAMEOVER;
        
        return true;
    }
}

void Stage::DrawStageClearScreen() {
    //스테이지 클리어 화면 출력
}

