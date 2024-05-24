#include <stdio.h>
#include <stdlib.h>

int detecter_coup_adv();
int detecter_mon_coup(t_return_code coup);

typedef struct Snake {
    int co_x;
    int co_y;
    int taille_snake;
    int* corp;
    } Snake;

typedef struct Arena {
    int a_x;
    int a_y;
    int nb_walls;
    int* matrice_walls;
    Snake snake;
    } Arena;
