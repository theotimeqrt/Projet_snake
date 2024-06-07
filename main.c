
// ######################################################## Programme bot snake 1v1 de Théotime QUIRET ################################################################

/*

Points forts :
- Intelligence du snake réglable (parametre générique N_force)
- programme découper en plusieurs fonctions (debug et amelioration facile)
- fonctions optimisés (crans d'arrêts efficaces)
- snake compétitif 

Points faibles :
- Gestion des murs de façon basique

*/

#include "snakeAPI.h"
#include "clientAPI.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "struct_decla.h"



// ==========================================Paramêtre de la force de l'esprit du snake======================================================================

#define N_force 12 // nombre de coups à anticiper
//10 marche très bien
//12 conseillé contre Super_player
//13 max testé, dirait qu'il s'embrouille aussi haut dans certain cas

// =================================================(+ grand + fort - rapide)================================================================================


// #########################################################################
// ############################################### MAIN ########################################################################
// #########################################################################


int main(void){
    printf("debut main\n") ;

    //void connectToServer(char* serverName, int port, char* name) //nom serveur : localhost, port : 1234
    connectToServer("localhost", 1234, "Master_T") ;
    printf("connecté serveur\n") ;
    // -----------------------Récuperation des informations------------------------------------

    char gameName ;
    int sizeX ;
    int sizeY ;
    int nbWalls ;

    waitForSnakeGame("TRAINING SUPER_PLAYER difficulty=0 timeout=5", &gameName, &sizeX, &sizeY, &nbWalls); //attend retour partie
    //void waitForSnakeGame(char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls)
    // TRAINING SUPER_PLAYER difficulty=2 timeout=5
    // TOURNAMENT Tournoi
    // rien pour 1v1
    printf("Nb walls %d\n", nbWalls);

    int walls[2000];
    int start = getSnakeArena(walls) ; // renvoi matrice des murs

    if(start == 0) {
        printf("tu commences\n");
    }
    else {
        printf("tu ne commences pas\n");
    }

    Arena arène;
    Snake snake;
    Snake adv ;

    // Je me cale sur les tailles données par le serveur, /!\ pas de coordonnées 0 /!\ 
    // coo (1;1) en bas à gauche de l'arène

    arène.a_x = sizeX;  // printf("sizeX : %d\n", sizeX); 
    arène.a_y = sizeY;  // printf("sizeY : %d\n", sizeY); 
    arène.nb_walls = nbWalls;
    arène.walls = walls;
    arène.snake = snake;
    arène.adv = adv;

    /*printf("Liste des murs :\n");
    for (int i = 0; i < 4 * arène.nb_walls; i += 4) {
        printf("Mur entre (%d, %d) et (%d, %d)\n",
               arène.walls[i], arène.walls[i + 1],
               arène.walls[i + 2], arène.walls[i + 3]);
    }*/

    // -------------------------- Positions serpents ---------------------------

    if (start == 0){ // si je commence je suis a gauche
        snake.coo_tete.x = 2;
        adv.coo_tete.x = sizeX - 3;
    }
    else{
        snake.coo_tete.x = sizeX - 3; // si l'adversaire commence je suis a droite 
        adv.coo_tete.x = 2; 
    }
    snake.coo_tete.y = sizeY / 2;
    adv.coo_tete.y = sizeY / 2;

    /* Debug de la position de départ
    printf("coo x du snake : %d\n", snake.coo_tete.x);
    printf("coo y du snake : %d\n", snake.coo_tete.y);
    printf("coo x de l'adversaire : %d\n", adv.coo_tete.x);
    printf("coo y de l'adversaire : %d\n", adv.coo_tete.y);
    */

    t_move coup_t_move ;
    t_return_code resultat_mon_coup ;
    
    int tour = -1;
    snake.taille_snake = 1;
    adv.taille_snake = 1;

    t_move move_adv;
    int sortie_adv;

    sendComment("Tu oses défier le Master T ?");
    printf("Puissance intelligente paramêtrée à : %d \n", N_force);

    while(1){

        tour ++ ;

        /* Regarder tailles evoluer dans le terminal
        printf("tour %d\n", tour) ; 
        printf("taille du serpent : %d \n", snake.taille_snake);
        printf("taille du serpent adv : %d \n", snake.taille_snake);
        */

        if (tour==0) {
            snake.taille_snake ++ ;
            adv.taille_snake ++ ;
        }

        else if(tour % 10 == 0) { // Snakes gagnent 1 de taille
            printf("Tour numero %d est divisible par 10\n", tour);
            snake.taille_snake ++ ;
            adv.taille_snake ++ ;

            // ---------Petits commentaires pour pas s'ennuyer durant partie ---------

        if ((snake.taille_snake==3))
        {
            sendComment("C'est parti");
        }

        if ((snake.taille_snake==5))
        {
            sendComment("Pas de ralentir !");
        }

        if ((snake.taille_snake==10))
        {
            sendComment("Je suis pas là pour poser du linot");
        }

        if ((snake.taille_snake==17))
        {
            sendComment("Je suis qu'à 17%% !");
        }

        if ((snake.taille_snake==23))
        {
            sendComment("T'es encore en vie ?");
        }

        if ((snake.taille_snake==35))
        {
            sendComment("Toujours pas de ralentir !");
        }

        if ((snake.taille_snake==50))
        {
            sendComment("Bien joué d'être arrivé ici");
        }

        if ((snake.taille_snake==70))
        {
            sendComment("Pas de ralentir, c'est tout droit !");
        }

        if ((snake.taille_snake==100))
        {
            sendComment("C'est long, c'est chiant, c'est Orléans!");
        }

        }

        printArena();


        // ################################ Cas 1, je commence #########################################

        if(start == 0){

                
                // ----------------------------Notre coup---------------------------------------------
                
                /* Version 1 du jeu manuel
                //printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
                //scanf("%d", &coup);
                //printf("Vous avez saisi la valeur %d pour coup\n", coup);
                */

                coup_t_move = recherche_super_coup(snake, arène);
                printf("Le mouv envoyé est : %d (0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest)\n", coup_t_move);

                resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup

                int sortie = detecter_mon_coup(resultat_mon_coup);

                if (sortie == 1){
                    return 0;
                }  

                // ---------------------------- Détecter coup adv --------------------------------


                sortie_adv = detecter_coup_adv(&move_adv); // Appel de la fonction modifiée

                if (sortie_adv) { // Vérification du résultat du coup
                    closeConnection();
                    return 0;
                } 
                 
        }

        // ################################ Cas 2, je suis a droite (de l'arène) #########################################

        else{

                // ---------------------------- Détecter coup adv --------------------------------


                sortie_adv = detecter_coup_adv(&move_adv); // Appel de la fonction modifiée

                if (sortie_adv) { // Vérification du résultat du coup
                    closeConnection();
                    return 0;
                }

                // ----------------------------Notre coup---------------------------------------------

                //printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
                //scanf("%d", &coup);
                //printf("Vous avez saisi la valeur %d pour coup\n", coup);
                coup_t_move = recherche_super_coup(snake, arène) ;
                printf("Le mouv envoyé est : %d (0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest)\n", coup_t_move);

                resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup

                int sortie = detecter_mon_coup(resultat_mon_coup);

                if (sortie == 1){
                    return 0;
                }        
        }

        // -------------------------------- fin des 2 cas de sens tours ---------------------------------


        // ################################### Mettre à jour notre du serpent #######################################""

        
        if (snake.taille_snake == 2) { // 1er tour, tete devient corps mais alloue aussi un tableau
            snake.corp = malloc(200 * sizeof(coo));
            snake.corp[0] = snake.coo_tete;
        }

        else {

            for (int i = (snake.taille_snake - 1); i > 0; i--){ // parcours le corps pour remplacer
                
                snake.corp[i] = snake.corp[i-1]; // tout se decale
                
                if(i ==1){
                    snake.corp[0] = snake.coo_tete; // tete devient 1er bout corps
                }
                    
            }
            
        }

        //Mettre à jour la tete du serpent

        if(coup_t_move == 0 ){
            snake.coo_tete.y -- ;
        }
        if (coup_t_move == 1){
            snake.coo_tete.x ++ ;
        }
        if (coup_t_move == 2){
            snake.coo_tete.y ++ ;
        }
        if (coup_t_move == 3){
            snake.coo_tete.x -- ;
        }

        

        /* Afficher le corps du serpent

        printf("coo snake : %d : %d \n", snake.coo_tete.x, snake.coo_tete.y);
        for(int i = 0; i < ((snake.taille_snake-1)); i++) {
            printf("%d, %d ; ", snake.corp[i].x,snake.corp[i].y);
        }

        */

        // ###################################### Mise à jour du serpent ADV #################################################""

        if (adv.taille_snake == 2) { // 1er tour, tete devient corps mais alloue aussi un tableau
            adv.corp = malloc( 200 * sizeof(coo));
            adv.corp[0] = adv.coo_tete;
        }

        else {


            for (int i = (adv.taille_snake - 1); i > 0; i--){ // parcours le corps pour remplacer
                
                adv.corp[i] = adv.corp[i-1]; // tout se decale
                
                if(i ==1){
                    adv.corp[0] = adv.coo_tete; // tete devient 1er bout corps
                }
                    
            }
            
        }

        //Mettre à jour la tete du serpent

        /* Rappel :
        typedef enum {
            NORTH = 0,
            EAST = 1,
            SOUTH = 2,
            WEST = 3
        } t_move; */
        

        if(move_adv == NORTH ){
            adv.coo_tete.y -- ;
        }
        if (move_adv == EAST){
            adv.coo_tete.x ++ ;
        }
        if (move_adv == SOUTH){
            adv.coo_tete.y ++ ;
        }
        if (move_adv == WEST){
            adv.coo_tete.x -- ;
        }

        /*Afficher le corps du serpent

        printf("coo tete adv : %d : %d \n", adv.coo_tete.x, adv.coo_tete.y);
        for(int i = 0; i < ((adv.taille_snake-1)); i++) {
            printf("%d, %d ; ", adv.corp[i].x,adv.corp[i].y);
        }

        */

        // ################################################# Mettre à jour l'arène ################################################

        arène.snake = snake;
        arène.adv = adv;




    } //fin boucle while

} // fin main


// #########################################################################
// ############################################### FONCTIONS ########################################################################
// #########################################################################

// --------------------sert à savoir si fin de jeu après coup adv--------------------------------

int detecter_coup_adv(t_move* coup_t_adv) { // ok
    t_return_code coup = getMove(coup_t_adv); // attend move adv

    int sortie = 0;
    if (coup == WINNING_MOVE) {
        printf("L'adv gagne !\n");
        sendComment("Ah");
        sortie = 1;
    } else if (coup == LOSING_MOVE) {
        printf("L'adv perd !\n");
        sendComment("Finito");
        sortie = 1;
    } else {
        sortie = 0;
    }

    
    return sortie;
}

// ------------------sert à savoir si fin jeu apres mon coup ---------------------

int detecter_mon_coup(t_return_code coup){ // ok

    if(coup == WINNING_MOVE){
        printf("Tu gagnes !\n");
        sendComment("Yeah !");
        closeConnection() ;
        return 1;
    }
    
    else if(coup == LOSING_MOVE){
        printf( "Tu perd !\n" );
        sendComment("classique mort");
        closeConnection() ;
        return 1;
    }
    else{
        return 0;
    }
}

// ------------------------Dit si le coup est possible en x,y------------------------

int coup_possible(int x, int y, Arena arène){ // coo de l'endroit a verifier -- ok

    if (x < 0 || x > (arène.a_x-1) || y < 0 || y > (arène.a_y-1)){
        return 0; // bordures
    }

    for (int i = 0 ; i< (arène.snake.taille_snake); i++){
        if (arène.snake.corp[i].x == x && arène.snake.corp[i].y == y){
            return 0; // mon snake
        }
    }

    for (int i = 0 ; i< (arène.adv.taille_snake); i++){
        if (arène.adv.corp[i].x == x && arène.adv.corp[i].y == y){
            return 0; // snake adv
        }
    }

    if ( (arène.adv.coo_tete.x == x && arène.adv.coo_tete.y == y)){
        return 0; // tete adv
    }


    return 1;
}

// -------------------Recherche de murs entre 2 cases de facon bête pas méchante----------------------

int recherche_simple_mur(int x1, int y1, int x2, int y2, Arena arène){ // ok
    for(int i = 0; i < (4*arène.nb_walls); i = i+4){ // parcours liste des murs
        if((x1 == arène.walls[i]) && (y1 == arène.walls[i+1]) && (x2 == arène.walls[i+2]) && (y2 == arène.walls[i+3])){
            return 1;
        }
        if((x2 == arène.walls[i]) && (y2 == arène.walls[i+1]) && (x1 == arène.walls[i+2]) && (y1 == arène.walls[i+3])){
            return 1;
        }
    }
    return 0; // aucun murs trouvés
}

// --------------Première version simple pour trouver un coup---------------------

t_move recherche_rapide_coup(Snake snake, Arena arène){ // coo de la tete du serpent --ok

    if(coup_possible(snake.coo_tete.x + 1,snake.coo_tete.y,arène)){
        //coup possible a l'est, voir murs
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x+1,snake.coo_tete.y,arène) == 0){
            // pas murs
            return EAST;
        }
    }

    if(coup_possible(snake.coo_tete.x - 1,snake.coo_tete.y,arène)){
        //coup possible a l'ouest, voir murs
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x-1,snake.coo_tete.y,arène) == 0){
            // pas murs
            return WEST;
        }
    }

    if(coup_possible(snake.coo_tete.x,snake.coo_tete.y - 1,arène)){
        //coup possible au nord, voir murs
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y-1,arène) == 0){
            // pas murs
            return NORTH;
        }
    }

    if(coup_possible(snake.coo_tete.x,snake.coo_tete.y + 1,arène)){
        //coup possible au sud, voir murs
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y+1,arène) == 0){
            // pas murs
            return SOUTH;
        }
    }

    // recherche_ rapide_coup echouée, pas de coo autour possibles
    return SOUTH;
}

// -----------------------Version avancée pour trouver un coup---------------------

t_move recherche_super_coup(Snake snake, Arena arène){

    // Force de coup initiale
    int coups_east = 0;
    int coups_west = 0;
    int coups_north = 0;
    int coups_south = 0;

    if((coup_possible(snake.coo_tete.x + 1,snake.coo_tete.y,arène)) && (recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x+1,snake.coo_tete.y,arène) == 0)){
        coo* tab_xy = malloc(200 * sizeof(coo));
        coups_east = coup_autour_case(snake.coo_tete.x + 1, snake.coo_tete.y, arène, N_force, tab_xy,0); // N générique du tout début pour la force
        free (tab_xy);
        printf("Force de %d à l'est \n",coups_east);
    }

    if((coup_possible(snake.coo_tete.x - 1,snake.coo_tete.y,arène))&&(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x-1,snake.coo_tete.y,arène) == 0) ){
        coo* tab_xy = malloc(200 * sizeof(coo));
        coups_west = coup_autour_case(snake.coo_tete.x - 1, snake.coo_tete.y, arène, N_force,tab_xy,0 );
        free (tab_xy);
        printf("Force de %d à l'ouest \n",coups_west);
    }

    if((coup_possible(snake.coo_tete.x,snake.coo_tete.y - 1,arène))&&(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y-1,arène) == 0)){
        coo* tab_xy = malloc(200 * sizeof(coo));
        coups_north = coup_autour_case(snake.coo_tete.x, snake.coo_tete.y - 1, arène, N_force,tab_xy,0);
        free (tab_xy);
        printf("Force de %d au nord \n",coups_north);
    }

    if((coup_possible(snake.coo_tete.x,snake.coo_tete.y + 1,arène))&&(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y+1,arène) == 0)){
        coo* tab_xy = malloc(200 * sizeof(coo));
        coups_south = coup_autour_case(snake.coo_tete.x, snake.coo_tete.y + 1, arène, N_force,tab_xy,0 );
        free (tab_xy);
        printf("Force de %d au sud \n",coups_south);
    }

    int max_coups = max(coups_east, coups_west, coups_north, coups_south);

    if (max_coups == coups_east){
        //coups l'est est = max
        return EAST;
    }
    if (max_coups == coups_west){
        //coups l'ouest est = max
        return WEST;
    }
    if (max_coups == coups_north){
        //coups le nord est = max
        return NORTH;
    }
    if (max_coups == coups_south){
        //coups le sud est = max
        return SOUTH;
    }

    //recherche_ super_coup echouée, pas de coo autour possibles, envoi south !
    return SOUTH;
}

// -------------------Recherche la puissance du coup----------------------


int coup_autour_case(int x_case, int y_case, Arena arena, int N, coo* tab_xy, int i){ // Le N choisis à combien de coups je regarde à l'avance
    int x = x_case;
    int y = y_case;
    int coups = 0;

    for (int j = 0; j < i; j++) {
        if (tab_xy[j].x == x && tab_xy[j].y == y) {
            return coups;
        }
    }

    tab_xy[i].x = x;
    tab_xy[i].y = y;
    i++;

    if (N == 0) { // cran d'arret, oui j'abuse des parentheses
        return coups;
    }
    
    if(coup_possible(x + 1, y, arena) && recherche_simple_mur(x, y, x+1, y, arena) == 0){
        coups = coups + 1 + coup_autour_case(x+1, y, arena, N-1,tab_xy, i );
    }
    if(coup_possible(x - 1, y, arena) && recherche_simple_mur(x, y, x-1, y, arena) == 0){
        coups = coups + 1 + coup_autour_case(x-1, y, arena, N-1,tab_xy, i);
    }
    if(coup_possible(x, y - 1, arena) && recherche_simple_mur(x, y, x, y-1, arena) == 0){
        coups = coups + 1 + coup_autour_case(x, y-1, arena, N-1,tab_xy ,i);
    }
    if(coup_possible(x, y + 1, arena) && recherche_simple_mur(x, y, x, y+1, arena) == 0){
        coups = coups + 1 + coup_autour_case(x, y+1, arena, N-1, tab_xy, i );
    }

    // printf("Il y a %d coups possibles en %d;%d \n", coups, x, y);
    return coups;
}

// ------------------Petite fonction max-------------------------

int max(int a, int b, int c, int d) { // petit max qui fait de mal à personne
    int max = a;
    if (b > max)
        max = b;
    if (c > max)
        max = c;
    if (d > max)
        max = d;
    return max;
}
// - FIN -