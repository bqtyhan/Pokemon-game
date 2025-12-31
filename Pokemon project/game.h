#ifndef GAME_H
#define GAME_H

#include "structs.h"

// Oyunu başlatan ana fonksiyon
void game(Player* player1, Player* player2);

// Tur mantığını yöneten fonksiyon
void round_logic(Player* p1, Player* p2);

void applyDamage(Player* owner, Pokemon* attacker, Pokemon* defender, Move* move);

#endif