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
    connectToServer("localhost", 1234, "Big_T") ;
    printf("connecté serveur\n") ;
    // -----------------------Récuperation des informations------------------------------------

    char gameName ;
    int sizeX ;
    int sizeY ;
    int nbWalls ;
    waitForSnakeGame("TRAINING SUPER_PLAYER difficulty=1 timeout=100 ", &gameName, &sizeX, &sizeY, &nbWalls); //attend retour partie
    //void waitForSnakeGame(char* gameType, char* gameName, int* sizeX, int* sizeY, int* nbWalls)
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

    arène.a_x = sizeX;  printf("sizeX : %d\n", sizeX); 
    arène.a_y = sizeY;  printf("sizeY : %d\n", sizeY); 
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


    printf("coo x du snake : %d\n", snake.coo_tete.x);
    printf("coo y du snake : %d\n", snake.coo_tete.y);
    printf("coo x de l'adversaire : %d\n", adv.coo_tete.x);
    printf("coo y de l'adversaire : %d\n", adv.coo_tete.y);

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
                printf("le mouv envoyé est : %d\n", coup_t_move);

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
                printf("Le mouv envoyé est : %d (0 pour Nord, 1 pour Est, 2 pour Sud, 3 pour Ouest)\n", coup_t_move);

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
            snake.corp = malloc(200 * sizeof(coo));
            snake.corp[0] = snake.coo_tete;
            //snake.corp[1] = snake.coo_y;
        }

        else {

            /*if (tour % 10 == 0) {
                snake.corp = realloc(snake.corp, (2*(snake.taille_snake-1)) * sizeof(int));
            }*/

            for (int i = (snake.taille_snake - 1); i > 0; i--){ // parcours le corps pour remplacer
                
                snake.corp[i] = snake.corp[i-1]; // tout se decale
                //snake.corp[i-1] = snake.corp[i-3];
                
                if(i ==1){
                    snake.corp[0] = snake.coo_tete; // tete devient 1er bout corps
                    //snake.corp[1] = snake.co_y;
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

        printf("coo snake : %d : %d \n", snake.coo_tete.x, snake.coo_tete.y);

        // Afficher le corps du serpent

        printf("affichage du corps du serpent :\n");
        for(int i = 0; i < ((snake.taille_snake-1)); i++) {
            printf("%d, %d ; ", snake.corp[i].x,snake.corp[i].y);

        }
        printf("\nfin corps serpent\n");

        // ###################################### Mise à jour du serpent ADV #################################################""

        if (adv.taille_snake == 2) { // 1er tour, tete devient corps mais alloue aussi un tableau
            adv.corp = malloc( 200 * sizeof(coo));
            adv.corp[0] = adv.coo_tete;
            //adv.corp[1] = adv.coo_tete.y;
        }

        else {

            /*if (tour % 10 == 0) {
                adv.corp = realloc(adv.corp, (2*(adv.taille_snake-1)) * sizeof(int));
            }*/

            for (int i = (adv.taille_snake - 1); i > 0; i--){ // parcours le corps pour remplacer
                
                adv.corp[i] = adv.corp[i-1]; // tout se decale
                //adv.corp[i-1] = adv.corp[i-3];
                
                if(i ==1){
                    adv.corp[0] = adv.coo_tete; // tete devient 1er bout corps
                    //adv.corp[1] = adv.coo_tete.y;
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

        printf("coo adv : %d : %d \n", adv.coo_tete.x, adv.coo_tete.y);

        // Afficher le corps du serpent

        printf("affichage du corps du serpent ADV :\n");
        for(int i = 0; i < ((adv.taille_snake-1)); i++) {
            printf("%d, %d ; ", adv.corp[i].x,adv.corp[i].y);

        }
        printf("\nfin corps serpent ADV\n");

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
        sendComment("classique mort");
        closeConnection() ;
        return 1;
    }
    else{
        return 0;
    }
}


int coup_possible(int x, int y, Arena arène){ // coo de l'endroit a verifier
    if (x < 0 || x > (arène.a_x-1) || y < 0 || y > (arène.a_y-1)){
        printf("bordures en %d %d \n",x,y);
        return 0; // bordures
    }
    for (int i = 0 ; i< (arène.snake.taille_snake); i++){
        if (arène.snake.corp[i].x == x && arène.snake.corp[i].y == y){
            printf("corp snake en %d %d \n",x,y);
            return 0; // mon snake
        }
        printf("pas corp snake en %d %d \n",x,y);
    }
    for (int i = 0 ; i< (arène.adv.taille_snake); i++){
        if (arène.adv.corp[i].x == x && arène.adv.corp[i].y == y){
            printf("corp adv en %d %d \n",x,y);
            return 0; // snake adv
        }
    }
    if ( (arène.adv.coo_tete.x == x && arène.adv.coo_tete.y == y)){
        printf("tete snake adv en %d %d \n",x,y);
        return 0;
    }

    printf("coup possible en %d %d \n",x,y);
    return 1;
}



t_move recherche_rapide_coup(Snake snake, Arena arène){ // coo de la tete du serpent
    if(coup_possible(snake.coo_tete.x + 1,snake.coo_tete.y,arène)){
        printf("coup possible a l'est, voir murs\n");
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x+1,snake.coo_tete.y,arène) == 0){
            return EAST;
        }
    }
    if(coup_possible(snake.coo_tete.x - 1,snake.coo_tete.y,arène)){
        printf("coup possible a l'ouest, voir murs\n");
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x-1,snake.coo_tete.y,arène) == 0){
            return WEST;
        }
    }
    if(coup_possible(snake.coo_tete.x,snake.coo_tete.y - 1,arène)){
        printf("coup possible au nord, voir murs\n");
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y-1,arène) == 0){
            return NORTH;
        }
    }
    if(coup_possible(snake.coo_tete.x,snake.coo_tete.y + 1,arène)){
        printf("coup possible au sud, voir murs\n");
        if(recherche_simple_mur(snake.coo_tete.x,snake.coo_tete.y,snake.coo_tete.x,snake.coo_tete.y+1,arène) == 0){
            return SOUTH;
        }
    }
    printf("recherche_ rapide_coup echouée, pas de coo autour possibles \n");
    return SOUTH;
}

int recherche_simple_mur(int x1, int y1, int x2, int y2, Arena arène){
    printf("Test murs entre %d %d et %d %d \n", x1,y1,x2,y2);
    for(int i = 0; i < (4*arène.nb_walls); i = i+4){ // parcours liste des murs
        if((x1 == arène.walls[i]) && (y1 == arène.walls[i+1]) && (x2 == arène.walls[i+2]) && (y2 == arène.walls[i+3])){
            return 1;
        }
        if((x2 == arène.walls[i]) && (y2 == arène.walls[i+1]) && (x1 == arène.walls[i+2]) && (y1 == arène.walls[i+3])){
            return 1;
        }
    }
    printf("Aucun mur trouvé \n");
    return 0;
}


// il ne se voit pas, se mange la queue a faire