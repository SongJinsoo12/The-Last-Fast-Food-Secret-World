#include "Stats.h"

Stats::Stats() {
	health = generalMaxHealth;
	turn = 0;
}

int Stats::GetHP() {
	return health;
}

