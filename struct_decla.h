#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
    int* walls;
    Snake snake;
    Snake adv;
    } Arena;

int detecter_coup_adv(t_move* coup_t_adv);
int detecter_mon_coup(t_return_code coup);

t_move recherche_rapide_coup(Snake snake, Arena arène);
int coup_possible(int x, int y, Arena arène);

int recherche_simple_mur(int x1, int y1, int x2, int y2, Arena arène);
