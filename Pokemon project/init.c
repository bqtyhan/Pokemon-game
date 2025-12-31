#include <stdio.h>
#include <stdlib.h> 
#include <time.h>   
#include <string.h> 
#include "init.h"

Type* findTypeByName(Type* types, char* typeName) {
    if (strcmp(typeName, "None") == 0 || strcmp(typeName, "-") == 0) {
        return NULL;
    }
    
    for (int i = 0; i < 18; i++) {
        if (strcmp(types[i].name, typeName) == 0) {
            return &types[i];
        }
    }
    return NULL; 
}

void initializeTypes(Type* types) {
    FILE* fp = fopen("types.txt", "r");
    if (fp == NULL) {
        printf("HATA: types.txt dosyasi acilamadi!\n");
        exit(1);
    }

    for (int i = 0; i < 18; i++) {
        fscanf(fp, "%s", types[i].name);
        
        for (int j = 0; j < 18; j++) {
            fscanf(fp, "%s %f", types[i].effects[j].defName, &types[i].effects[j].multiplier);
            strcpy(types[i].effects[j].atkName, types[i].name);
        }

        strcpy(types[i].effects[18].defName, "None");
        types[i].effects[18].multiplier = 1.0; 
        strcpy(types[i].effects[18].atkName, types[i].name);
        // ---------------------------------------------
    }
    fclose(fp);
    printf("Types initialized.\n");
}

void initializeMoves(Move* moves, Type* types) {
    FILE* fp = fopen("moves.txt", "r");
    if (fp == NULL) {
        printf("HATA: moves.txt dosyasi acilamadi!\n");
        exit(1);
    }

    for (int i = 0; i < 486; i++) {
        char typeNameTemp[50];    
        char categoryTemp[50];    
        float tempPower;          

        // moves[i].name direkt struct içine, diğerleri geçici değişkenlere
        fscanf(fp, "%s %s %s %f", moves[i].name, typeNameTemp, categoryTemp, &tempPower);

        moves[i].power = (int)tempPower;

        // "Fire" yazısını Type struct'ıyla eşleştiriyoruz
        Type* foundType = findTypeByName(types, typeNameTemp);
        if (foundType != NULL) {
            moves[i].type = *foundType; // Bulunan türü kopyala
        }

        if (strcmp(categoryTemp, "Physical") == 0) {
            moves[i].category = PHYSICAL;
        } else {
            moves[i].category = SPECIAL;
        }
    }

    fclose(fp);
    printf("Moves initialized.\n");
}

void initializePokemons(Pokemon* pokemons, Type* types, Move* moves) {
    FILE* fp = fopen("pokemon.txt", "r");
    if (fp == NULL) {
        printf("HATA: pokemon.txt dosyasi acilamadi!\n");
        exit(1);
    }

    for (int i = 0; i < 1015; i++) {
        char type1Name[50], type2Name[50];
        fscanf(fp, "%s %s %s %d %d %d %d %d %d", 
            pokemons[i].name, type1Name, type2Name, 
            &pokemons[i].maxHP, &pokemons[i].attack, &pokemons[i].defense, 
            &pokemons[i].spAtk, &pokemons[i].spDef, &pokemons[i].speed);
        
        // Başlangıç canını fulle
        pokemons[i].currentHP = pokemons[i].maxHP;
        
        // Türleri eşleştir
        Type* t1 = findTypeByName(types, type1Name);
        if (t1 != NULL) pokemons[i].types[0] = *t1;

        if (strcmp(type2Name, "-") == 0 || strcmp(type2Name, "None") == 0) {
            strcpy(pokemons[i].types[1].name, "None");
        } else {
            Type* t2 = findTypeByName(types, type2Name);
            if (t2 != NULL) pokemons[i].types[1] = *t2;
        }

        // Rastgele 4 Saldırı Seçimi (Benzersiz)
        int selectedIndices[4] = {-1, -1, -1, -1};
        for (int m = 0; m < 4; m++) {
            int rIndex;
            int isUnique;
            do {
                isUnique = 1;
                rIndex = rand() % 486; 
                for (int check = 0; check < m; check++) {
                    if (selectedIndices[check] == rIndex) {
                        isUnique = 0;
                        break;
                    }
                }
            } while (!isUnique);
            selectedIndices[m] = rIndex;
            pokemons[i].moves[m] = moves[rIndex];
        }
    }
    fclose(fp);
    printf("Pokemons initialized.\n");
}

void initialize(Type* types, Move* moves, Pokemon* pokemons, Player* player1, Player* player2) {
    srand(time(NULL));
    printf("Yukleme basliyor...\n");

    initializeTypes(types);
    initializeMoves(moves, types);
    initializePokemons(pokemons, types, moves);

    
    printf("Player 1 Name: ");
    
    if (fgets(player1->name, 50, stdin) != NULL) {
        player1->name[strcspn(player1->name, "\n")] = 0;
    }

    printf("Player 2 Name: ");
    
    if (fgets(player2->name, 50, stdin) != NULL) {
        player2->name[strcspn(player2->name, "\n")] = 0;
    }

    player1->currentIndex = 0;
    player2->currentIndex = 0;

    int p1Indices[6] = {-1, -1, -1, -1, -1, -1};
    for(int i=0; i<6; i++) {
        int rIndex, isUnique;
        do {
            isUnique = 1;
            rIndex = rand() % 1015; 
            for(int k=0; k<i; k++) {
                if(p1Indices[k] == rIndex) isUnique = 0;
            }
        } while(!isUnique);
        p1Indices[i] = rIndex;
        player1->pokemons[i] = pokemons[rIndex];
    }

    int p2Indices[6] = {-1, -1, -1, -1, -1, -1};
    for(int i=0; i<6; i++) {
        int rIndex, isUnique;
        do {
            isUnique = 1;
            rIndex = rand() % 1015;
            for(int k=0; k<i; k++) {
                if(p2Indices[k] == rIndex) isUnique = 0;
            }
        } while(!isUnique);
        p2Indices[i] = rIndex;
        player2->pokemons[i] = pokemons[rIndex];
    }
    printf("Initialization complete.\n");
}