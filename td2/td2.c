#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

pthread_mutex_t mymutex_px;
pthread_mutex_t mymutex_py;

float * px = NULL;
float * py = NULL;

// Fonction lancee par le thread 1
static void * affiche_max_fois_valeur_px (void * p_data)
{
	int max = (int) p_data;

	// Boucle de traitement
	int i;
	for (i = 0; i < max; i++) {

		pthread_mutex_lock(&mymutex_px);
		
		// Affichage de la valeur pointee par px
		printf("[thread1] %f\n", *px);
		pthread_mutex_unlock(&mymutex_px);

		sleep(1);
	}

	// Preparation du texte a retourner au parent
	char *resultChild = malloc(20);
	strcpy(resultChild, "statusFinChild");

	// Retour au parent
	pthread_exit(resultChild);
 
   	return NULL;
}

// Fonction lancee par le thread 2 
static void * affiche_max_fois_valeur_py (void * p_data)
{
	int max = (int) p_data;

	// Boucle de traitement
	int i;
	for (i = 0; i < max; i++) {

		pthread_mutex_lock(&mymutex_py);
		
		// Affichage de la valeur pointee par px
		printf("[thread2] %f\n", *py);

		pthread_mutex_unlock(&mymutex_py);
		
		sleep(1);
	}

	// Preparation du texte a retourner au parent
	char *resultChild = malloc(20);
	strcpy(resultChild, "statusFinChild");

	// Retour au parent
	pthread_exit(resultChild);
 
   	return NULL;
}

// Fonction lancee par le thread 3 
static void * affiche_max_fois_valeur_px_py (void * p_data)
{
	int max = (int) p_data;

	// Boucle de traitement
	int i;
	for (i = 0; i < max; i++) {

		int allMutexOk = 0;
		while (allMutexOk == 0) {
			pthread_mutex_lock(&mymutex_px);
			 
			if (pthread_mutex_trylock(&mymutex_py) == 0) {
				allMutexOk = 1;
			}
			else {
				pthread_mutex_unlock(&mymutex_px);
			}
		}
			
		// Affichage de la valeur pointee par px
		printf("[thread3] %f %f\n", *px, *py);
		
		pthread_mutex_unlock(&mymutex_px);
		pthread_mutex_unlock(&mymutex_py);
		
		sleep(1);
	}

	// Preparation du texte a retourner au parent
	char *resultChild = malloc(20);
	strcpy(resultChild, "statusFinChild");

	// Retour au parent
	pthread_exit(resultChild);
 
   	return NULL;
}

int main(int argc, char *argv[])
{

	// Parse les arguments

	if (argc < 2) {
		printf("erreur de parametres\n");
		return -1;
	}

	int max = atoi(argv[1]);

	// Initialise les mutex
	pthread_mutex_init(&mymutex_px, NULL);
	pthread_mutex_init(&mymutex_py, NULL);

	// Declaration du reel
	float x = 1;
	float y = 2;

	// Affectations
	px = &x;
	py = &y;

        // Lance un thread qui manipule px 
	pthread_t myThread1;
	int ret = pthread_create (
            & myThread1, NULL,
            affiche_max_fois_valeur_px, (void *) max
         );	

        // Lance un thread qui manipule py 
	pthread_t myThread2;
	ret = pthread_create (
            & myThread2, NULL,
            affiche_max_fois_valeur_py, (void *) max
         );	

        // Lance un thread qui manipule px et py 
	pthread_t myThread3;
	ret = pthread_create (
            & myThread3, NULL,
            affiche_max_fois_valeur_px_py, (void *) max
         );	

	// Boucle de 0 à max
	int i;
	for (i = 0; i < max; i++) {
		
		pthread_mutex_lock(&mymutex_px);

		px = NULL;
		printf("[main] %i\n", i);
		px = &x;

		pthread_mutex_unlock(&mymutex_px);
	}

	printf("termine\n");
        
	// Attente de la fin des thread fils
        pthread_exit(NULL);

	// Detruit les mutex
	pthread_mutex_destroy(&mymutex_px);
	pthread_mutex_destroy(&mymutex_py);

	return(0);
}
