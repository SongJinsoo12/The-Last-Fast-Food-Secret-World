#include "stage.h"
#include "item.h"
#include "MainGame.h"






int Stage::Get_LargeStage() {
    return LargeStage;
}

int Stage::Get_SmallStage() {
    return SmallStage;
}

//void Stage::BindGame(MainGame* game)
//{
//    this->game = game;
//}



bool Stage::GameStart() {
    //GetStage();
    SmallStage = 1;
    LargeStage = 1;
    GameState = E_PVP;
    return true;
}
bool TutorialStage::CheckTutorial() {
    if ( Get_LargeStage() <= 3 && Get_SmallStage() <= 3)
    {
        //Maingame.h GameStage = E_TUTORIAL;?
        return IsTutorial = true;
    }
    else
    {
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


bool Stage::IsLastBossStage() { 
    if (LargeStage == 6 && SmallStage == 5)
        return true;
} //라보 스테이지 체크

//CType TutorialStage::TutorialCard() {
//    if (CheckTutorial()) {
//
//        if (Get_LargeStage() == 1 && Get_SmallStage() == 1)
//            return E_Attack;
//
//        if (Get_LargeStage() == 1 && Get_SmallStage() == 2)
//            return E_Deffence;
//
//        if (Get_LargeStage() == 1 && Get_SmallStage() == 3)
//            return E_Magic;
//    }
//    
//    // 카드.h enum
//}

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


    if (!isMonsterLost) //몬스터 hp = 0 또는 몬스터 비활성화일떄 else 코드 실행
    {
        GameState = E_PVP;
        return GameState;
    }
    else
    {
        GameState = E_STAGE_CLEAR; 
        if (IsLastBossStage)
            return 7;
    }

    AddGold(setgold());
    //보상 화면출력 함수?
    GameState = E_MENU;
    return GameState;

}
//if (LargeStage == 6 && SmallStage == 5) //월드 6-5
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
    if (StageClear(/* ? */) == 7) {
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

