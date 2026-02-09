#pragma once

class CardPlayerStatus
{
public:
	CardPlayerStatus();
	~CardPlayerStatus();
	int GetHP();
	void SetHP(int p_hp);
	bool GetIsActAval();
	void SetIsActAval(bool p_isActAval);
private:

protected:
	int hp; //체력
	bool isActAval; //행동 가능 여부
};