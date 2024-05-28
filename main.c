//      main

//prof
#include "snakeAPI.h"
#include "clientAPI.h"

//basiques
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//mes autres
#include "struct_decla.h"

//#define RESET   "\033[0m" // utilisation de IA Mistral ici pour trouver les codes couleurs
//#define YELLOW  "\033[33m"
//#define RED     "\033[31m"

/*      Lancer tunnel : 
ssh -N -p 22 21111206@pc5056.polytech.upmc.fr -L 1234:pc5039:1234 -L 8080:pc5039:8080
puis mettre mdp sorbonne
faire autre terminal
visible sur internet en parrallele
*/

/*
Le serveur de jeu tourne sur la machine pc5039 (port 1234), qui est accessible depuis les machines de l'école. 
Si vous voulez utiliser votre propre machine, il faudra mettre en place un tunnel SSH en passant par 
la passerelle de l'école pc5056.polytech.upmc.fr (cela se fait avec la commande 
ssh -N -p 22 <login>@pc5056.polytech.upmc.fr -L 1234:pc5039:1234& 
où <login> est votre login de Sorbonne Université; le mot de passe associé doit vous être demandé au moment de créer le tunnel)
Donc utiliser 
ssh -N -p 22 21111206@pc5056.polytech.upmc.fr -L 1234:pc5039:1234 -L 8080:pc5039:8080
ps | grep ssh 
car faudrait kill process, marche pas encore

j'ai obtenu 18304

Les 6 derniers caracteres sont la seed de l'arène


*/





// #########################################################################
// ############################################### MAIN ########################################################################
// #########################################################################


int main(void){
    printf("debut main\n") ;

    //void connectToServer(char* serverName, int port, char* name) //nom serveur : localhost, port : 1234
    connectToServer("localhost", 1234, "le_T4") ;
    printf("connecté serveur\n") ;
    // -----------------------Récuperation des informations------------------------------------

    char gameName ;
    int sizeX ;
    int sizeY ;
    int nbWalls ;
    waitForSnakeGame("TRAINING RANDOM_PLAYER difficulty=2 timeout=100 ", &gameName, &sizeX, &sizeY, &nbWalls); //attend retour partie
    //void waitForSnakeGame(char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls)

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

    arène.a_x = sizeX;  printf("sizeX : %d\n", sizeX); 
    arène.a_y = sizeY;  printf("sizeY : %d\n", sizeY); 
    arène.nb_walls = nbWalls;
    arène.walls = walls;
    arène.snake = snake;
    arène.adv = adv;

    // -------------------------- Positions serpents ---------------------------

    if (start == 0){ // si je commence je suis a gauche
        snake.co_x = 3;
        adv.co_x = sizeX - 2;
    }
    else{
        snake.co_x = sizeX - 2; // si l'adversaire commence je suis a droite 
        adv.co_x = 3; 
    }

    if (sizeY % 2 == 0){ // si impair, le centre de l'arène et la position est +1 
        snake.co_y = sizeY / 2;
        adv.co_y = sizeY / 2;
    }
    else{
        snake.co_y = (sizeY + 1) / 2;
        adv.co_y = (sizeY + 1) / 2;
    }

    printf("coo x du snake : %d\n", snake.co_x);
    printf("coo y du snake : %d\n", snake.co_y);
    printf("coo x de l'adversaire : %d\n", adv.co_x);
    printf("coo y de l'adversaire : %d\n", adv.co_y);

    t_move coup_t_move ;
    t_return_code resultat_mon_coup ;
    
    int tour = -1;
    snake.taille_snake = 1;
    adv.taille_snake = 1;

    t_move move_adv;
    int sortie_adv;
    

    while(1){


                
        tour ++ ;
        printf("tour %d\n", tour) ; 
        printf("taille du serpent : %d \n", snake.taille_snake);
        printf("taille du serpent adv : %d \n", snake.taille_snake);

        if (tour==0) {
            snake.taille_snake ++ ;
            adv.taille_snake ++ ;
        }

        else if(tour % 10 == 0) {
            printf("Tour numero %d est divisible par 10\n", tour);
            snake.taille_snake ++ ;
            adv.taille_snake ++ ;
        }

        //printf("tests tours passés\n");

        printArena();

        //printf("print arena passé\n");

        // ################################ Cas 1, je commence #########################################

        if(start == 0){

                printf("entre dans start = 0\n"); 
                
                // ----------------------------Notre coup---------------------------------------------
                
                //printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
                //scanf("%d", &coup);
                //printf("Vous avez saisi la valeur %d pour coup\n", coup);
                coup_t_move = recherche_rapide_coup(snake, arène) ;
                printf("La valeur de la direction est : %d\n", coup_t_move);

                resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup
                printf("mouv envoyé\n");

                int sortie = detecter_mon_coup(resultat_mon_coup);

                if (sortie == 1){
                    return 0;
                }  

                // ---------------------------- Détecter coup adv --------------------------------

                printf("avant detection coup adv\n");
                sortie_adv = detecter_coup_adv(&move_adv); // Appel de la fonction modifiée
                printf("apres\n");
                if (sortie_adv) { // Vérification du résultat du coup
                    closeConnection();
                    return 0;
                } 
                 
        }

        // ################################ Cas 2, je suis a droite (de l'arène) #########################################

        else{

                printf("entre dans start = 1\n");

                // ---------------------------- Détecter coup adv --------------------------------

                printf("avant detection coup adv\n");
                sortie_adv = detecter_coup_adv(&move_adv); // Appel de la fonction modifiée
                printf("apres\n");
                if (sortie_adv) { // Vérification du résultat du coup
                    closeConnection();
                    return 0;
                }

                // ----------------------------Notre coup---------------------------------------------

                //printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
                //scanf("%d", &coup);
                //printf("Vous avez saisi la valeur %d pour coup\n", coup);
                coup_t_move = recherche_rapide_coup(snake, arène) ;
                printf("La valeur de la direction est : %d\n", coup_t_move);

                resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup
                printf("mouv envoyé\n");

                int sortie = detecter_mon_coup(resultat_mon_coup);

                if (sortie == 1){
                    return 0;
                }        
        }

        // -------------------------------- fin des 2 cas de sens tours ---------------------------------


        // ################################### Mettre à jour notre du serpent #######################################""

        
        if (snake.taille_snake == 2) { // 1er tour, tete devient corps mais alloue aussi un tableau
            snake.corp = malloc(2 * sizeof(int));
            snake.corp[0] = snake.co_x;
            snake.corp[1] = snake.co_y;
        }

        else {

            if (tour % 10 == 0) {
                snake.corp = realloc(snake.corp, (2*(snake.taille_snake-1)) * sizeof(int));
            }

            for (int i = 2*(snake.taille_snake - 1); i > 0; i= i-2){ // parcours le corps pour remplacer
                
                
                snake.corp[i] = snake.corp[i-2]; // tout se decale
                snake.corp[i-1] = snake.corp[i-3];
                
                if(i ==2){
                    snake.corp[0] = snake.co_x; // tete devient 1er bout corps
                    snake.corp[1] = snake.co_y;
                }
                    
            }
            
        }

        //Mettre à jour la tete du serpent

        if(coup_t_move == 0 ){
            snake.co_y ++ ;
        }
        if (coup_t_move == 1){
            snake.co_x ++ ;
        }
        if (coup_t_move == 2){
            snake.co_y -- ;
        }
        if (coup_t_move == 3){
            snake.co_x -- ;
        }

        printf("coo snake : %d : %d \n", snake.co_x, snake.co_y);

        // Afficher le corps du serpent

        printf("affichage du corps du serpent :\n");
        for(int i = 0; i < (2*(snake.taille_snake-1)); i++) {
            printf("%d, ", snake.corp[i]);
        }
        printf("\nfin corps serpent\n");

        // ###################################### Mise à jour du serpent ADV #################################################""

        if (adv.taille_snake == 2) { // 1er tour, tete devient corps mais alloue aussi un tableau
            adv.corp = malloc(2 * sizeof(int));
            adv.corp[0] = adv.co_x;
            adv.corp[1] = adv.co_y;
        }

        else {

            if (tour % 10 == 0) {
                adv.corp = realloc(adv.corp, (2*(adv.taille_snake-1)) * sizeof(int));
            }

            for (int i = 2*(adv.taille_snake - 1); i > 0; i= i-2){ // parcours le corps pour remplacer
                
                
                adv.corp[i] = adv.corp[i-2]; // tout se decale
                adv.corp[i-1] = adv.corp[i-3];
                
                if(i ==2){
                    adv.corp[0] = adv.co_x; // tete devient 1er bout corps
                    adv.corp[1] = adv.co_y;
                }
                    
            }
            
        }

        //Mettre à jour la tete du serpent

        /* Definition of a move 
        typedef enum {
            NORTH = 0,
            EAST = 1,
            SOUTH = 2,
            WEST = 3
        } t_move; */
        

        if(move_adv == NORTH ){
            adv.co_y ++ ;
        }
        if (move_adv == EAST){
            adv.co_x ++ ;
        }
        if (move_adv == SOUTH){
            adv.co_y -- ;
        }
        if (move_adv == WEST){
            adv.co_x -- ;
        }

        printf("coo adv : %d : %d \n", adv.co_x, adv.co_y);

        // Afficher le corps du serpent

        printf("affichage du corps du serpent adv :\n");
        for(int i = 0; i < (2*(adv.taille_snake-1)); i++) {
            printf("%d, ", adv.corp[i]);
        }
        printf("\nfin corps serpent adv\n");

        // ################################################# Mettre à jour l'arène ################################################

        arène.snake = snake;
        arène.adv = adv;




    } //fin boucle while

} // fin main


// #########################################################################
// ############################################### FONCTIONS ########################################################################
// #########################################################################


int detecter_coup_adv(t_move* coup_t_adv) {
    t_return_code coup = getMove(coup_t_adv); // attend move adv

    int sortie = 0;
    if (coup == WINNING_MOVE) {
        printf("L'adv gagne !\n");
        sendComment("Ah");
        sortie = 1;
    } else if (coup == LOSING_MOVE) {
        printf("L'adv perd !\n");
        sendComment("Pas de ralentir");
        sortie = 1;
    } else {
        sortie = 0;
    }

    
    return sortie;
}

int detecter_mon_coup(t_return_code coup){

    if(coup == WINNING_MOVE){
        printf("Tu gagnes !\n");
        sendComment("Yeah !");
        closeConnection() ;
        return 1;
    }
    
    else if(coup == LOSING_MOVE){
        printf( "Tu perd !\n" );
        sendComment("classique death");
        closeConnection() ;
        return 1;
    }
    else{
        return 0;
    }
}

int coup_possible(int x, int y, Arena arène){ // coo de l'endroit a verifier
    if (x < 1 || x > arène.a_x || y < 1 || y > arène.a_y){
        return 0; // bordures
    }
    for (int i = 0 ; i< (arène.snake.taille_snake); i=i+2){
        if (arène.snake.corp[i] == x && arène.snake.corp[i+1] == y){
            return 0; // propre snake
        }
    }
    for (int i = 0 ; i< (arène.adv.taille_snake); i=i+2){
        if (arène.adv.corp[i] == x && arène.adv.corp[i+1] == y){
            return 0; // snake adv
        }
    }
    return 1;
}

t_move recherche_rapide_coup(Snake snake, Arena arène){ // coo de la tete du serpent
    if(coup_possible(snake.co_x + 1,snake.co_y,arène)){
        if(recherche_simple_mur(snake.co_x,snake.co_y,snake.co_x+1,snake.co_y,arène) == 0){
            return EAST;
        }
    }
    if(coup_possible(snake.co_x - 1,snake.co_y,arène)){
        if(recherche_simple_mur(snake.co_x,snake.co_y,snake.co_x-1,snake.co_y,arène) == 0){
            return WEST;
        }
    }
    if(coup_possible(snake.co_x,snake.co_y + 1,arène)){
        if(recherche_simple_mur(snake.co_x,snake.co_y,snake.co_x,snake.co_y+1,arène) == 0){
            return NORTH;
        }
    }
    if(coup_possible(snake.co_x,snake.co_y - 1,arène)){
        if(recherche_simple_mur(snake.co_x,snake.co_y,snake.co_x,snake.co_y-1,arène) == 0){
            return SOUTH;
        }
    }
    printf("recherche_ rapide_coup echouée, pas de coo autour possibles \n");
    return SOUTH;
}

int recherche_simple_mur(int x1, int y1, int x2, int y2, Arena arène){
    for(int i = 0; i < arène.nb_walls; i = i+4){ // parcours liste des murs
        if((x1 == arène.walls[i]) && (y1 == arène.walls[i+1]) && (x2 == arène.walls[i+2]) && (y2 == arène.walls[i+3])){
            return 1;
        }
        if((x2 == arène.walls[i]) && (y2 == arène.walls[i+1]) && (x1 == arène.walls[i+2]) && (y1 == arène.walls[i+3])){
            return 1;
        }
    }
    return 0;
    printf("Aucun mur trouvé \n");
}
