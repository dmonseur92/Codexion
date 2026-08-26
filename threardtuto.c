#include <pthread.h>
#include <stdio.h>

typedef struct
{
    char *pokemon1;
    char *pokemon2;
} Combat;


void	*fight(void *arg)
{
	// Prends l'adresse contenue dans arg et considère-la comme l'adresse d'un Combat
	Combat *combat = (Combat *)arg;

	printf("%s VS %s\n",
        combat->pokemon1,
        combat->pokemon2);
	return NULL;
}

int main()
{
	Combat combat1 = {"Pikachu", "Salamèche"};
	Combat combat2 = {"Carapuce", "Bulbizarre"};

	pthread_t thread1;
	pthread_t thread2;

	//pthread_create(&thread, NULL, function, DATA(args for fnc));
	pthread_create(&thread1, NULL, fight, &combat1);
	pthread_create(&thread2, NULL, fight, &combat2);

	// wait for threads to finish
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	//otherwise this might be executed before threads are over
	return (0);
}

