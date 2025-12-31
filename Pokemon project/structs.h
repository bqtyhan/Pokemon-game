#ifndef STRUCTS_H  
#define STRUCTS_H

typedef enum {
    PHYSICAL,
    SPECIAL
} Category;

typedef struct {
    char atkName[50];
    char defName[50];
    float multiplier;
} TypeEffect;

typedef struct {
    char name[50];
    TypeEffect effects[19]; 
} Type;

typedef struct {
    char name[50];
    Type type;           
    Category category;   
    int power;
} Move;

typedef struct {
    char name[50];
    Type types[2];       
    int maxHP;
    int currentHP;
    int attack;
    int defense;
    int spAtk;
    int spDef;
    int speed;
    Move moves[4];      
} Pokemon;

typedef struct {
    char name[50];
    Pokemon pokemons[6]; 
    int currentIndex;    
} Player;

#endif 