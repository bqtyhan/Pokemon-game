#include <stdio.h>
#include <string.h>
#include "game.h"

// YARDIMCI FONKSİYON: Bir oyuncunun hayatta kalan pokemonu var mı?
int hasAlivePokemon(Player* p) {
    for (int i = 0; i < 6; i++) {
        if (p->pokemons[i].currentHP > 0) {
            return 1;
        }
    }
    return 0;
}

// YARDIMCI FONKSİYON: Çarpan bulucu 
float getMultiplier(Type type, char* defTypeName) {
    for (int i = 0; i < 19; i++) { 
        if (strcmp(type.effects[i].defName, defTypeName) == 0) {
            return type.effects[i].multiplier;
        }
    }
    return 1.0; 
}
void applyDamage(Player* owner, Pokemon* attacker, Pokemon* defender, Move* move) {
    float A, D;
    
    if (move->category == PHYSICAL) {
        A = (float)attacker->attack;
        D = (float)defender->defense;
    } else {
        A = (float)attacker->spAtk;
        D = (float)defender->spDef;
    }

    float typeEffect1 = getMultiplier(move->type, defender->types[0].name);
    float typeEffect2 = getMultiplier(move->type, defender->types[1].name);

    float STAB = 1.0;
    if (strcmp(move->type.name, attacker->types[0].name) == 0 || 
        strcmp(move->type.name, attacker->types[1].name) == 0) {
        STAB = 1.5;
    }

    float damageCalc = (float)move->power * (A / D) * typeEffect1 * typeEffect2 * STAB;
    int damage = (int)damageCalc; 

    int oldHP = defender->currentHP;
    defender->currentHP -= damage;
    if (defender->currentHP < 0) {
        defender->currentHP = 0;
    }
    
    printf("\n========================================\n");
    printf(" [ATTACK] %s's %s attacks!\n", owner->name, attacker->name);
    printf(" Move: %s (Pwr: %d, Type: %s)\n", move->name, move->power, move->type.name);
    printf("----------------------------------------\n");
    
    float totalEffect = typeEffect1 * typeEffect2;
    
    if (totalEffect > 1.0) printf(" >>> IT'S SUPER EFFECTIVE! <<<\n");
    if (totalEffect < 1.0 && totalEffect > 0) printf(" >>> It's not very effective... <<<\n");
    if (totalEffect == 0) printf(" >>> IT HAD NO EFFECT! <<<\n");

    if(STAB > 1.0) printf(" (STAB Bonus Applied)\n");

    printf(" Damage Dealt: %d\n", damage);
    printf(" %s HP: %d -> %d\n", defender->name, oldHP, defender->currentHP);
    printf("========================================\n");
}

void makeChoice(Player* p, int* action, int* subChoice) {
    Pokemon* currentPoke = &p->pokemons[p->currentIndex];
    printf("\n--- %s's Turn (%s) ---\n", p->name, currentPoke->name);
    printf("1- Attack\n");
    printf("2- Change Pokemon\n");
    
    do {
        printf("Choice: ");
        scanf("%d", action);
        if (*action != 1 && *action != 2) {
            printf("Gecersiz secim! Lutfen 1 veya 2 girin.\n");
        }
    } while (*action != 1 && *action != 2);

    if (*action == 1) { 
        printf("Select a move:\n");
        for (int i = 0; i < 4; i++) {
            printf("%d- %s (Pwr: %d, Type: %s)\n", 
                i+1, currentPoke->moves[i].name, 
                currentPoke->moves[i].power, 
                currentPoke->moves[i].type.name);
        }
        
        int moveChoice;
        do {
            printf("Move: ");
            scanf("%d", &moveChoice);
            if (moveChoice < 1 || moveChoice > 4) {
                 printf("Gecersiz hamle! 1-4 arasi secin.\n");
            }
        } while (moveChoice < 1 || moveChoice > 4);
        
        *subChoice = moveChoice - 1; 

    } else if (*action == 2) { 
        printf("Select a Pokemon to switch:\n");
        for (int i = 0; i < 6; i++) {
            if (p->pokemons[i].currentHP > 0) {
                printf("%d- %s (HP: %d)\n", i+1, p->pokemons[i].name, p->pokemons[i].currentHP);
            }
        }
        
        int pokeChoice;
        do {
             printf("Pokemon: ");
             scanf("%d", &pokeChoice);
             if (pokeChoice < 1 || pokeChoice > 6) {
                 printf("Gecersiz pokemon! 1-6 arasi secin.\n");
             }
        } while (pokeChoice < 1 || pokeChoice > 6);
        *subChoice = pokeChoice - 1; 
    }
}

void round_logic(Player* p1, Player* p2) { 
    int p1Action, p1Sub;
    int p2Action, p2Sub;

    makeChoice(p1, &p1Action, &p1Sub);
    // Eğer P1 Pokemon değiştirdiyse hemen uygula
    if (p1Action == 2) {
        p1->currentIndex = p1Sub;
        printf("%s switched to %s!\n", p1->name, p1->pokemons[p1->currentIndex].name);
    }

    makeChoice(p2, &p2Action, &p2Sub);
    // Eğer P2 Pokemon değiştirdiyse hemen uygula
    if (p2Action == 2) {
        p2->currentIndex = p2Sub;
        printf("%s switched to %s!\n", p2->name, p2->pokemons[p2->currentIndex].name);
    }
    
    Pokemon* poke1 = &p1->pokemons[p1->currentIndex];
    Pokemon* poke2 = &p2->pokemons[p2->currentIndex];
    
    // Savaş Senaryoları
    if (p1Action == 2 && p2Action == 2) {
        // İkisi de değiştirdiyse saldırı olmaz, yukarıda zaten değiştirdik.
        printf("\nBoth players focused on switching.\n");
    }
    else if (p1Action == 2 && p2Action == 1) {
        // P1 değiştirdi, P2 saldırıyor (P2 avantajlı, yeni gelene vurur)
        applyDamage(p2, poke2, poke1, &poke2->moves[p2Sub]);
    }
    else if (p1Action == 1 && p2Action == 2) {
        // P2 değiştirdi, P1 saldırıyor
        applyDamage(p1, poke1, poke2, &poke1->moves[p1Sub]);
    }
    else {
        // İkisi de SALDIRIYOR (Hız kontrolü lazım)
        Player* firstPlayer;  Player* secondPlayer;
        Pokemon* firstPoke;   Pokemon* secondPoke;
        Move* firstMove;      Move* secondMove;

        // Hız kontrolü
        if (poke1->speed >= poke2->speed) {
            firstPlayer = p1; secondPlayer = p2;
            firstPoke = poke1; secondPoke = poke2;
            firstMove = &poke1->moves[p1Sub]; secondMove = &poke2->moves[p2Sub];
        } else {
            firstPlayer = p2; secondPlayer = p1;
            firstPoke = poke2; secondPoke = poke1;
            firstMove = &poke2->moves[p2Sub]; secondMove = &poke1->moves[p1Sub];
        }

        // Önce hızlı olan vurur
        applyDamage(firstPlayer, firstPoke, secondPoke, firstMove);
        
        // Eğer ikinci Pokemon ölmediyse, o da vurur
        if (secondPoke->currentHP > 0) {
            applyDamage(secondPlayer, secondPoke, firstPoke, secondMove);
        } else {
            printf("\n%s fainted! %s cannot attack back.\n", secondPoke->name, secondPlayer->name);
        }
    }


    if (p1->pokemons[p1->currentIndex].currentHP <= 0) {
        printf("\n!!! %s's %s fainted! !!!\n", p1->name, p1->pokemons[p1->currentIndex].name);
        int found = 0;
        for(int i=0; i<6; i++) {
            if(p1->pokemons[i].currentHP > 0) {
                p1->currentIndex = i;
                printf("%s sent out %s!\n", p1->name, p1->pokemons[i].name);
                found = 1;
                break;
            }
        }
        if(!found) printf("%s has no Pokemon left!\n", p1->name);
    }
    
    if (p2->pokemons[p2->currentIndex].currentHP <= 0) {
        printf("\n!!! %s's %s fainted! !!!\n", p2->name, p2->pokemons[p2->currentIndex].name);
        int found = 0;
        for(int i=0; i<6; i++) {
            if(p2->pokemons[i].currentHP > 0) {
                p2->currentIndex = i;
                printf("%s sent out %s!\n", p2->name, p2->pokemons[i].name);
                found = 1;
                break;
            }
        }
        if(!found) printf("%s has no Pokemon left!\n", p2->name);
    }
}

void game(Player* player1, Player* player2) {
    printf("\n#################################\n");
    printf("#         BATTLE START          #\n");
    printf("#################################\n");
    
    int roundCount = 1;
    while (hasAlivePokemon(player1) && hasAlivePokemon(player2)) {
        printf("\n\n >>>>>>> ROUND %d <<<<<<<\n", roundCount++);
        round_logic(player1, player2); 
    }

    printf("\n#################################\n");
    printf("#          GAME OVER            #\n");
    printf("#################################\n");
    
    if (hasAlivePokemon(player1)) {
        printf("\n   WINNER: %s!   \n", player1->name);
    } else {
        printf("\n   WINNER: %s!   \n", player2->name);
    }
}