#include "stage.h"
#include "item.h"
#include "MainGame.h"


//int Stage::GL_Stage(MainGame & game) { return L_stage = game.GetLargeStage(); } XXXX

//int Stage::GS_Stage(MainGame & game) { return S_stage = game.GetLargeStage(); }XXXX



//GetSmallStage() 


//void Stage::BindGame(MainGame* game)
//{
//    this->game = game;
//}

//MainGame.h SetStage() <-?

bool Stage::GameStart() {
    //GetStage();
    S_stage = 1;
    L_stage = 1;
    GameState = E_START;
    return true;
}
bool TutorialStage::CheckTutorial() {
    if (L_stage <= 3 && S_stage <= 3)
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
    if (S_stage % 5 == 0)
        return L_stage * S_stage * 100; //보스
    return L_stage * S_stage * 50; // 잡몹
}

//
//{
// 카드박스 드롭 함수?
//}

//void Stage::Init(int large, int small)
//{
//    L_Stage = large;
//    smallStage = small;
//}

bool Stage::IsBossStage(const MainGame & game) {
    //return S_stage == 5;
}

CType TutorialStage::TutorialCard() {
    if (IsTutorial)
        if (L_stage == 1 && S_stage == 1)
            return E_Attack;

    if (L_stage == 1 && S_stage == 2)
        return E_Deffence;

    if (L_stage == 1 && S_stage == 3)
        return E_Magic;

    // 카드.h enum
}

bool Stage::StageStart() {
    return true;
}

bool Stage::StageClear(bool isMonsterDead) {


    if (!isMonsterDead) //몬스터 hp = 0 또는 몬스터 비활성화일때 거짓
    {
        GameState = E_PVP;
        return false;
    }
    else
    {
        GameState = E_STAGE_CLEAR; // 성공 시
        if (L_stage == 6 && S_stage == 5) //월드 6-5
            //

            if (S_stage != 5)
                S_stage++;
            else
            {
                L_stage++;
                S_stage = 1;
            }
    }

    AddGold(setgold());
    return true;

}

bool Stage::GameClear() {

}

bool Stage::GameOver(bool isPlayerDead) {

    if (!isPlayerDead) { //플레이어 hp = 0

        return false;
    }
    else
    {
        GameState = E_GAMEOVER;
        //L_stage = -1;
        return true;
    }
}

