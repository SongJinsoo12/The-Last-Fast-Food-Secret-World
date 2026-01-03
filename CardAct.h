#pragma once
#include <Windows.h>
#include "Card.h"

void DrawLine(HDC hdc, int startX, int startY, int lengthX, int lengthY);
void DrawBG(HDC hdc, RECT rect);
void DrawDeckCount(HDC hdc, Deck& playerHand, Deck& enemyHand, RECT rect);
