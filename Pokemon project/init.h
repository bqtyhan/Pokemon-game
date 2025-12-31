#ifndef INIT_H
#define INIT_H

#include "structs.h" 

void initialize(Type* types, Move* moves, Pokemon* pokemons, Player* player1, Player* player2);
void initializeTypes(Type* types);

void initializeMoves(Move* moves, Type* types);

void initializePokemons(Pokemon* pokemons, Type* types, Move* moves);

#endif