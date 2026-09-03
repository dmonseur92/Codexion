#include <pthread.h>
#include <stdio.h>

typedef struct
{
    int pokemon_available;
    pthread_mutex_t mutex; // Le verrou partagé par tous les threads
} Game;

void *catch_pokemon(void *arg)
{
    Game *game = (Game *)arg;

    // Prend le verrou : un seul thread peut entrer ici à la fois
    pthread_mutex_lock(&game->mutex);

    // On protège cette vérification + modification
    // pour éviter que deux threads attrapent le même Pokemon
    if (game->pokemon_available)
    {
        printf("Dresseur attrape le Pokemon !\n");
        game->pokemon_available = 0;
    }
    else
    {
        printf("Trop tard, le Pokemon est deja pris !\n");
    }

    // Libère le verrou pour que l'autre thread puisse continuer
    pthread_mutex_unlock(&game->mutex);

    return NULL;
}

int main(void)
{
    Game game;

    game.pokemon_available = 1;

    // Initialise le mutex avant de l'utiliser
    pthread_mutex_init(&game.mutex, NULL);

    pthread_t trainer1;
    pthread_t trainer2;

    // Les deux threads reçoivent le même Game
    // donc ils partagent le même Pokemon ET le même mutex
    pthread_create(&trainer1, NULL, catch_pokemon, &game);
    pthread_create(&trainer2, NULL, catch_pokemon, &game);

    // On attend que les deux threads aient terminé
    pthread_join(trainer1, NULL);
    pthread_join(trainer2, NULL);

    // On détruit le mutex quand plus aucun thread ne l'utilise
    pthread_mutex_destroy(&game.mutex);

    return 0;
}
