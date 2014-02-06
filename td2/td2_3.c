#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

pthread_mutex_t mymutex;
float * px = NULL;

// Fonction lancee par le thread
static void * affiche_max_fois_valeur_px (void * p_data)
{
	int max = (int) p_data;

	// Boucle de traitement
	int i;
	for (i = 0; i < max; i++) {

		pthread_mutex_lock(&mymutex);
		
		// Affichage de la valeur pointee par px
		printf("[thread] %f\n", *px);

		pthread_mutex_unlock(&mymutex);
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

	// Initialise le mutex
	pthread_mutex_init(&mymutex, NULL);

	// Declaration du reel
	float x = 1;

	// Affectation de px avec l'adresse de x
	px = &x;

        // Lance un thread qui affichera "et mon courroux"
	pthread_t myThread;
	int ret = pthread_create (
            & myThread, NULL,
            affiche_max_fois_valeur_px, (void *) max
         );	

	// Boucle de 0 à max
	int i;
	for (i = 0; i < max; i++) {
		
		pthread_mutex_lock(&mymutex);

		px = NULL;
		printf("[main] %i\n", i);
		px = &x;

		pthread_mutex_unlock(&mymutex);
	}

	printf("termine\n");
        
	// Annulation du thread fils
	//pthread_cancel(myThread);

	// Attente de la fin du thread fils
	char* retChildThread;
        pthread_join(myThread, (void**)&retChildThread);

	printf("child ended : %s\n", retChildThread);

	// Detruit le mutex
	pthread_mutex_destroy(&mymutex);

	return(0);
}
