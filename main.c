#include <stdio.h>
#include <stdlib.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

typedef struct tVector{
    int nX;
    int nY;
}tVector;

typedef struct tActor{
    SDL_Texture* txActor;
    tVector vect;
    int height;
    int width;
}tActor;

typedef struct elementListe {
    tActor* donnee;
    struct elementListe* next;
}element;

typedef struct ListeRepere {
    element* debut;
    element* fin;
    int taille;
}tListDyn;

void init(tListDyn* list);
int siListVide(tListDyn* list, tActor* donnee);
int insertList(tListDyn* list, SDL_Renderer** renderer);
void vue(SDL_Renderer** renderer, tListDyn* list);

int main(int argc, char* argv[])
//BUT : créer une liste dynamique FIFO avc tActor et coder la vue en SDL
//ENTREE : des types structurés, des fonctions et des procédures
//SORTIE : une vue de la liste dynamique en SDL
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0){ // Démarrage de la SDL. Si erreur :
        printf("Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }

    SDL_Window* fnt = NULL;

    fnt = SDL_CreateWindow("TDA", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    if( fnt == NULL ){
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        getchar();
        SDL_Quit();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fnt, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface *img = NULL;

    SDL_Event event;

    tListDyn *list = NULL;
    tActor *tempDonnee = NULL;

    int nCpt=0;

    if ((list = (tListDyn *) malloc (sizeof(tListDyn))) == NULL)
        return -1;
    if ((tempDonnee = (tActor *) malloc (sizeof(tActor))) == NULL)
        return -1;

    init(list);

    for(nCpt = 0;nCpt < 2;nCpt++){
        if (nCpt == 0){
            tempDonnee->vect.nX = 100;
            tempDonnee->vect.nY = 100;
            tempDonnee->width = 600;
            tempDonnee->height = 400;

            img = IMG_Load("./img/ggwp.bmp");
            tempDonnee->txActor = SDL_CreateTextureFromSurface(renderer,img);
            SDL_FreeSurface(img);

            siListVide(list, tempDonnee);
            printf("test");
        }

        else if (nCpt == 1){
            insertList (list,&renderer);
            printf("test2");
        }
    }

    vue(&renderer, list);
    SDL_RenderPresent(renderer);

    int quit = 1;

    while(quit == 1){
        if(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT){
                SDL_Quit();
                break;
            }
        }
    }

    return 0;
}

void init(tListDyn* list)
{
    list->debut = NULL;
    list->fin = NULL;
    list->taille = 0;
}

int siListVide(tListDyn* list, tActor* donnee)
{
    element* addElement;

    if ((addElement = (element *) malloc (sizeof (element))) == NULL)
        return -1;
    if ((addElement->donnee = (tActor *) malloc (50 * sizeof (tActor))) == NULL)
        return -1;

    addElement->donnee = donnee;
    addElement->next = NULL;
    list->debut = addElement;
    list->fin = addElement;
    list->taille++;

    return 0;
}

int insertList(tListDyn* list, SDL_Renderer** renderer)
{
    SDL_Surface* img = NULL;
    element* addElement;

    if ((addElement = (element *) malloc (sizeof (element))) == NULL)
        return -1;
    if ((addElement->donnee = (tActor *) malloc (sizeof (tActor))) == NULL)
        return -1;

    addElement->donnee->vect.nX = 0;
    addElement->donnee->vect.nY = 0;
    addElement->donnee->width = 800;
    addElement->donnee->height = 600;

    img = IMG_Load("./img/welcome.bmp");
    addElement->donnee->txActor = SDL_CreateTextureFromSurface(*renderer,img);

    SDL_FreeSurface(img);


    addElement->next = list->debut;
    list->debut = addElement;
    list->taille++;

    return 0;
}

void vue(SDL_Renderer** renderer, tListDyn* list)
{
    SDL_Rect pos;
    element* courant;

    courant = list->debut;

    while (courant != NULL){
        pos.x = courant->donnee->vect.nX;
        pos.y = courant->donnee->vect.nY;
        pos.w = courant->donnee->width;
        pos.h = courant->donnee->height;

        SDL_RenderCopy(*renderer,courant->donnee->txActor,NULL,&pos);

        courant = courant->next;
    }
}
