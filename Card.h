#pragma once
enum CAttribute {
	E_BULGOGI,
	E_SOURCE,
	E_CHESSE,
	E_VEGAT,
	E_BREAD
};

enum CType {
	E_Attack,
	E_Deffence,
	E_Magic
};

class Card
{
protected:
	int Atk;
	int Rdc;
	CAttribute Ait;
	CType Type;
public:
	//
	int x, y, num;

public:
	Card();

	//카드정보 초기화(카드X, 카드Y, 카드구분용번호)
	void SetCard(CType p_type, int p_x, int p_y, int p_num)
	{
		this->Type = p_type;
		this->x = p_x, this->y = p_y, this->num = p_num;
	}
};

