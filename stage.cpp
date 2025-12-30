#include "stage.h"
#include "item.h"
#include "MainGame.h"
//#include gold.h"?


GetLargeStage() 
GetSmallStage() 


bool Stage::GameStart() {
    //GetStage();
    largeStage = 1;
    smallStage = 1;
    return true;
}
bool Stage::TutorialStart() {
    return true;
}

int Stage::setgold() {
    if (GetSmallStage() % 5 == 0)
        return largeStage * GetSmallStage() * 100; //보스
    return largeStage * smallStage * 50; // 잡몹
}

void Stage::Init(int large, int small)
{
    largeStage = large;
    smallStage = small;
}

bool Stage::IsBossStage( const MainGame& game) {
    return game.GetLargeStage() == 6 && game.GetSmallStage() == 5;
}

CType Stage::TutorialCard() {
        if (largeStage = 1 && smallStage == 1)
           return  E_Attack;
        
        if (largeStage == 1 && smallStage == 2)
            return E_Deffence;
        
        if (largeStage == 1 && smallStage == 3)
            return E_Magic;
        
    // 카드.h enum
}

bool Stage::StageStart() {
    return true;
}

bool Stage::StageClear(bool isMonsterDead, MainGame& game) {


    if (!isMonsterDead) //몬스터 hp = 0 또는 몬스터 비활성화
    {
        return false;
    }
    else
    {
        if (largeStage == 6 && smallStage == 5)
            

        if (smallStage !=5)
        smallStage++;
        else
        {
            largeStage++;
            smallStage = 1;
        }
    }
    game.AddGold(setgold());
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
        largeStage = -1;
        return true;
    }
}

