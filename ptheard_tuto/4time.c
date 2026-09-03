#define _DEFAULT_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct t_game
{
	int pokemon_available;
	int has_pokemon_spawed;
	pthread_mutex_t mutex;
	pthread_cond_t pokemon_ready;
	struct timespec start;
	struct timespec end;
} Game;

typedef struct t_trainer
{
	char *name;
	Game *game;
} Trainer;

void *catch_pokemon(void *arg)
{
	Trainer	*trainer = (Trainer *)arg;
	Game *game = trainer->game;

	pthread_mutex_lock(&game->mutex);
	while (!game->has_pokemon_spawed)
		pthread_cond_wait(&game->pokemon_ready, &game->mutex);
	clock_gettime(CLOCK_REALTIME, &game->end);
	printf("%ld s: ", game->end.tv_nsec - game->start.tv_nsec);
	if (game->pokemon_available)
	{
		printf("Le pokemon est capture par %s !!!\n", trainer->name);
		game->pokemon_available -= 1;
	}
	else
		printf("Le pokemon n' est plus la ... %s a le seum ;(\n", trainer->name);
	pthread_mutex_unlock(&game->mutex);
	return NULL;
}

void *spaw_pokemon(void *arg)
{
	Game *game = (Game *)arg;

	pthread_mutex_lock(&game->mutex);
	game->has_pokemon_spawed = 1;
	game->pokemon_available += 1;
	clock_gettime(CLOCK_REALTIME, &game->end);
	printf("%ld s: A wild pokemon apppears\n", game->end.tv_nsec - game->start.tv_nsec);
	pthread_cond_broadcast(&game->pokemon_ready);
	pthread_mutex_unlock(&game->mutex);
	return NULL;
}

int	main()
{
	Game	game;
	Trainer	sasha = {"sasha", &game};
	Trainer	team_rocket = {"team_rocket", &game};

	pthread_t	trainer1;
	pthread_t	trainer2;
	pthread_t	professor;

	game.pokemon_available = 0;
	game.has_pokemon_spawed = 0;
	clock_gettime(CLOCK_REALTIME, &game.start);
	pthread_mutex_init(&game.mutex, NULL);
	pthread_cond_init(&game.pokemon_ready, NULL);
	pthread_create(&trainer1, NULL, catch_pokemon, &sasha);
	pthread_create(&trainer2, NULL, catch_pokemon, &team_rocket);
	usleep(1000000);
	pthread_create(&professor, NULL, spaw_pokemon, &game);
	pthread_join(trainer1, NULL);
	pthread_join(trainer2, NULL);
	pthread_join(professor, NULL);
	pthread_mutex_destroy(&game.mutex);
	pthread_cond_destroy(&game.pokemon_ready);
	return (0);
}
