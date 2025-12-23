#pragma once
enum CAttribute {
	BULGOGI,
	SOURCE,
	CHESSE,
	VEGAT,
	BREAD
};

enum CType {
	Attack,
	Deffence,
	Magic
};

class Card
{
protected:
	int Atk;
	int Rdc;
	CAttribute Ait;
	CType Type;
};

