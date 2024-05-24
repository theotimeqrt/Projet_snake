//      main

//prof
#include "snakeAPI.h"
#include "clientAPI.h"

//basiques
#include <stdio.h>
#include <stdlib.h>

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
// ############################################### FONCTIONS ########################################################################
// #########################################################################


int detecter_coup_adv(){

    t_move coup_t_move;
    t_return_code coup = getMove(&coup_t_move); // attend move adv 

    if(coup == WINNING_MOVE){
        printf("L'adv gagne !\n");
        sendComment("Ah");
        closeConnection() ;
        return 1;
    }
    
    else if(coup == LOSING_MOVE){
        printf( "L'adv perd !\n" );
        sendComment("Pas de ralentir");
        closeConnection() ;
        return 1;
    }
    else{
        return 0;
    }
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

// #########################################################################
// ############################################### MAIN ########################################################################
// #########################################################################


int main(void){

    //void connectToServer(char* serverName, int port, char* name) //nom serveur : localhost, port : 1234
    connectToServer("localhost", 1234, "le_T") ;

    // -----------------------Récuperation des informations------------------------------------

    char gameName ;
    int sizeX ;
    int sizeY ;
    int nbWalls ;
    waitForSnakeGame("TRAINING RANDOM_PLAYER difficulty=3 timeout=100 ", &gameName, &sizeX, &sizeY, &nbWalls); //attend retour partie
    //void waitForSnakeGame(char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls)

    int walls[2000];
    int tour = getSnakeArena(walls) ; // renvoi matrice des murs
    if(tour == 0) 
        printf("tu commences\n") ;
    else 
        printf("tu ne commences pas\n") ;
    printf("print arena fait\n");

    Arena arène;
    Snake snake;

    // Je me cale sur les tailles données par le serveur, /!\ pas de coordonnées 0 /!\ 
    // coo (1;1) en bas à gauche de l'arène

    arène.a_x = sizeX;  printf("sizeX : %d\n", sizeX); 
    arène.a_y = sizeY;  printf("sizeY : %d\n", sizeY); 
    arène.nb_walls = nbWalls;
    arène.matrice_walls = walls;
    arène.snake = snake;

    // --------------------------Position initiale serpent---------------------------

    if (tour == 0){ // si je commence je suis a gauche
        snake.co_x = 3;
    }
    else{
        snake.co_x = sizeX - 2; // si l'adversaire commence je suis a droite  
    }

    if (sizeY % 2 == 0){ // si impair, le centre de l'arène et la position est +1 
        snake.co_y = sizeY / 2;
    }
    else{
        snake.co_y = (sizeY + 1) / 2;
    }

    printf("coo x du snake : %d\n", snake.co_x);
    printf("coo y du snake : %d\n", snake.co_y);

    t_move coup_t_move ;
    t_return_code resultat_mon_coup ;

    // ################################ Cas 1, je commence #########################################

    if(tour == 0){
        while(1){

            printf("entre dans boucle tour = 0\n");
            printArena();

            // ----------------------------Notre coup---------------------------------------------
            
            int coup = 0;
            
            printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
            scanf("%d", &coup);
            printf("Vous avez saisi la valeur %d pour coup\n", coup);


            resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup
            printf("mouv envoyé\n");

            coup_t_move = (t_move)coup ; // cast
            printf("avant detection notre coup\n");
            int sortie = detecter_mon_coup(resultat_mon_coup);
            printf("apres\n");
            if (sortie == 1){
                return 0;
            }


            // ---------------------------- Détecter coup adv --------------------------------

            int sortie_adv = 0; 

            printf("avant detection coup adv\n");
            sortie_adv = detecter_coup_adv();
            printf("apres\n");
            if (sortie_adv == 1){
                return 0;
            }
            
            // --------------------------------Mettre à jour les coordonnées du serpent-----------------------------

            printf("avant maj coo serpent\n");

            if(coup == 0 ){
                snake.co_y ++ ;
            }
            if (coup == 1){
                snake.co_x ++ ;
            }
            if (coup == 2){
                snake.co_y -- ;
            }
            if (coup == 3){
                snake.co_x -- ;
            }

            printf("coo x du snake : %d\n", snake.co_x);
            printf("coo y du snake : %d\n", snake.co_y);

            // ---------------------------- Mettre à jour l'arène --------------------------------------------------

            
            arène.snake = snake;

        }
    }

    // ################################ Cas 2, je suis a droite (de l'arène) #########################################


    else {
        while(1){

            printf("entre dans boucle tour = 1\n");
            printArena();

            // ---------------------------- Détecter coup adv --------------------------------

            int sortie_adv = 0; 

            printf("avant detection coup adv\n");
            sortie_adv = detecter_coup_adv();
            printf("apres\n");
            if (sortie_adv == 1){
                return 0;
            }

            // ----------------------------Notre coup---------------------------------------------

            int coup = 0;
            
            printf("Entrez la valeur de coup : 0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest\n");
            scanf("%d", &coup);
            printf("Vous avez saisi la valeur %d pour coup\n", coup);


            resultat_mon_coup = sendMove(coup_t_move); // envoi mon coup
            printf("mouv envoyé\n");

            coup_t_move = (t_move)coup ; // cast
            printf("avant detection notre coup\n");
            int sortie = detecter_mon_coup(resultat_mon_coup);
            printf("apres\n");
            if (sortie == 1){
                return 0;
            }

            // --------------------------------Mettre à jour les coordonnées du serpent-----------------------------

            printf("avant maj coo serpent\n");

            if(coup == 0 ){
                snake.co_y ++ ;
            }
            if (coup == 1){
                snake.co_x ++ ;
            }
            if (coup == 2){
                snake.co_y -- ;
            }
            if (coup == 3){
                snake.co_x -- ;
            }

            printf("coo x du snake : %d\n", snake.co_x);
            printf("coo y du snake : %d\n", snake.co_y);

            // ---------------------------- Mettre à jour l'arène --------------------------------------------------

            
            arène.snake = snake;
            


        }
    }

}


