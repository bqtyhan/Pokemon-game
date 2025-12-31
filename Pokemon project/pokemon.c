#include <stdio.h> 
#include "structs.h" 
#include "init.h"
#include "game.h"

int main() {
    setbuf(stdout, NULL); 

    static Type Types[18];
    static Move Moves[486];
    static Pokemon Pokemons[1015];
    
    Player Player1;
    Player Player2;

    initialize(Types, Moves, Pokemons, &Player1, &Player2);
    
    game(&Player1, &Player2);

    return 0;
}