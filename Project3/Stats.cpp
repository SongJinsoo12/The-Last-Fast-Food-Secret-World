#include "Stats.h"

template <typename T>
Stats<T>::Stats() {
	health = generalMaxHealth;
	turn = 0;
}

template <typename T>
int Stats<T>::GetHP() {
	return health;
}