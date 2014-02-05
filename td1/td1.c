#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>

// Fonction lancee par le thread
static void * affiche_n_fois_courroux (void * p_data)
{
	int nb = (int) p_data;

	// Boucle de traitement
	int i;
	for (i = 0; i < nb; i++) {
		printf("et mon courroux\n");
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

	printf("termine\n");
        
	// Annulation du thread fils
	pthread_cancel(myThread);

	// Attente de la fin du thread fils
	//char* retChildThread;
        //pthread_join(myThread, (void**)&retChildThread);

	//printf("child ended : %s\n", retChildThread);

	return(0);
}
