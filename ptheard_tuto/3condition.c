#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct
{
    int pokemon_available;
    int has_pokemon_spawned;
    pthread_mutex_t mutex;
    pthread_cond_t pokemon_ready;

} Game;

typedef struct
{
    char *name;
    Game *game;
} Trainer;


void *catch_pokemon(void *arg)
{
    Trainer *trainer = (Trainer *)arg;
    Game *game = trainer->game;

    pthread_mutex_lock(&game->mutex);

    while (!game->has_pokemon_spawned)
        pthread_cond_wait(&game->pokemon_ready, &game->mutex);
        // Attend et libère temporairement le mutex, puis le reprend au réveil.

    if (game->pokemon_available)
    {
        printf("Le pokemon est capture par %s\n", trainer->name);
        game->pokemon_available -= 1;
    }
    else
        printf("Le pokemon est deja pris, %s a le seum\n", trainer->name);

    pthread_mutex_unlock(&game->mutex);
    return NULL;
}


void *spawn_pokemon(void *arg)
{
    Game *game = (Game *)arg;

    pthread_mutex_lock(&game->mutex);

    game->pokemon_available += 1;
    game->has_pokemon_spawned = 1;

    pthread_cond_broadcast(&game->pokemon_ready);
    // Réveille tous les threads qui attendent sur cette condition.
	// pthread_cond_signal(&game->pokemon_ready);
	// Réveille un seul thread au random qui attend sur cette condition.
    pthread_mutex_unlock(&game->mutex);

    return NULL;
}


int main(void)
{
    Game game;
    Trainer sasha = {"Sasha", &game};
    Trainer team_rocket = {"Team Rocket", &game};

    pthread_t dresseur1;
    pthread_t dresseur2;
    pthread_t professor;

    game.pokemon_available = 0;
    game.has_pokemon_spawned = 0;

    printf("pokemon available: %d\n", game.pokemon_available);

    pthread_mutex_init(&game.mutex, NULL);
    pthread_cond_init(&game.pokemon_ready, NULL);

    pthread_create(&dresseur1, NULL, catch_pokemon, &sasha);
    pthread_create(&dresseur2, NULL, catch_pokemon, &team_rocket);

    usleep(1000000);

    pthread_create(&professor, NULL, spawn_pokemon, &game);

    pthread_join(dresseur1, NULL);
    pthread_join(dresseur2, NULL);
    pthread_join(professor, NULL);

    pthread_cond_destroy(&game.pokemon_ready);
    pthread_mutex_destroy(&game.mutex);

    return 0;
}
