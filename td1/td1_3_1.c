#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

// Fonction lancee par le thread
static void * affiche_n_fois_courroux (void * p_data)
{
	int nb = (int) p_data;

	int i;
	for (i = 0; i < nb; i++) {
		printf("et mon courroux\n");
		sleep(1);
	}

	pthread_exit(NULL);
 
   	return NULL;
}

int main(int argc, char *argv[])
{

	// Parse les arguments

	if (argc < 3) {
		printf("erreur de parametres\n");
		return -1;
	}

	int n = atoi(argv[1]);
	int m = atoi(argv[2]);

	printf("M=%i main/coucou\n", m);
	printf("N=%i thread/et mon courroux\n", n);
	printf("\n");

        // Lance un thread qui affichera "et mon courroux"
	pthread_t myThread;
	int ret = pthread_create (
            & myThread, NULL,
            affiche_n_fois_courroux, (void *) n
         );	

        // Le main affiche m fois "coucou

	int i;
	for (i = 0; i < m; i++) {
		printf("coucou\n");
		sleep(1);
	}

	printf("termine");

        pthread_exit(NULL);

	return(0);
}
