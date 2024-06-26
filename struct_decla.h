#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*      Lancer tunnel : 
ssh -N -p 22 21111206@pc5056.polytech.upmc.fr -L 3456:pc5039:3456 -L 8080:pc5039:8080
puis mdp sorbonne
*/

typedef struct coo { // mise à jour suite au CM pour pas utiliser des int x2
    int x;
    int y;
    } coo;

typedef struct Snake {
    coo coo_tete; // juste coo de la tête
    int taille_snake;
    coo* corp; // tableau de coordonnées
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

int recherche_simple_mur(int x1, int y1, int x2, int y2, Arena arène);
int coup_possible(int x, int y, Arena arène);

t_move recherche_super_coup(Snake snake, Arena arène);


int coup_autour_case(int x, int y, Arena arène, int N, coo* tab_xy, int i);

int max(int a, int b, int c, int d);
