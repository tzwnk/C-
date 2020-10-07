#include "p3100080-p3150126-pizza1.h"

unsigned int cooks =Ncook;
unsigned int ovens = Noven;
int sum = 0;
unsigned int seed;

void *order(void *threadId) {

	struct timespec start,finish;
	//Starting time.
	clock_gettime(CLOCK_REALTIME, &start); 

	int *tid,rc,pizza;
	tid = (int *)threadId;
	//number of pizza
	pizza = rand_r(&seed)%5+1;
	printf("Hello From Order %d Which Has %d Pizzas!\n\n", *tid,pizza);
	
	rc = pthread_mutex_lock(&mymutex);
	while(cooks==0){
		printf("The Order: %d Has No Available Cook. Blocked...\n\n", *tid);
		rc = pthread_cond_wait(&mycond, &mymutex);
	}
	while(ovens==0){
		printf("The Order: %d Has No Available Oven. Blocked...\n\n", *tid);
		rc = pthread_cond_wait(&mycond, &mymutex);
	}
	cooks--;
	ovens--;	
	printf(" The Order %d Is In Progress\n\n", *tid);
	//Preparation time
	sleep(1*pizza);
	rc = pthread_mutex_unlock(&mymutex);

	//Bake time
	sleep(10);

	rc = pthread_mutex_lock(&mymutex);
	clock_gettime(CLOCK_REALTIME, &finish);
    int seconds = finish.tv_sec - start.tv_sec; 
    printf("\nThe Order %d was succesfully completed in: %d min. \n\n", *tid,seconds);
    ++cooks;
    ++ovens;
    sum = sum + seconds;
    printf("The sum of minutes up until Order %d is: %d min. \n", *tid, sum );
    printf("The average waiting time per customer so far is: %d min. \n\n", sum/ *tid);
    rc = pthread_cond_signal(&mycond);
    rc = pthread_mutex_unlock(&mymutex);		
	pthread_exit(tid);
}

int main(int argc, char **argv)
{	

	//Check the arguments
	if(argc!=3){
		printf(" Error: We Need Two Arguments \n");			
		exit(-1);
	}
	//Save user's input 
	int numOfThreads = atoi(argv[1]);
	seed = atoi(argv[2]);

	//Check if number is negative
	if(numOfThreads<=0 || seed<0){
		printf(" %d,%d : Wrong Input Numbers \n",numOfThreads,seed);	
		exit(-1);
	}
	//Printing the input of user
	printf("\n We Will Create %d Threads \n",numOfThreads);	
	

	//Allocate memory
	pthread_t *threads;										
	threads = malloc(numOfThreads*sizeof(pthread_t));

	//Check if memory allocated
	if (threads==NULL){
		printf(" Memory Not Allocated \n");					
		exit(-1);
	}
	//Malloc is done 
	printf(" Memory Allocated \n\n");							

	//Declarations
	int rc;
	int id[numOfThreads];
	pthread_mutex_init(&mymutex, NULL);
    pthread_cond_init(&mycond, NULL);

    //For loop creator
   	for(int i = 0; i < numOfThreads; i++) {    		
		printf(" Main is Creating Thread %d\n", i+1);
		id[i] = i + 1;
		//Thread Creator
   		rc = pthread_create(&threads[i], NULL, order, &id[i]);
   		sleep(rand_r(&seed)%7+1);
		//Check if thread created correctly
		if (rc != 0) {
			printf("ERROR: return code from pthread_create() is %d\n", rc);
       		exit(-1);
       		}
    }

    void *status;
    for (int i = 0; i < numOfThreads; i++) {
		rc = pthread_join(threads[i], &status);
		
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}

		printf(" Main: Thread %d Returned %d as Status Code. \n", id[i], (*(int *)status));
	}

	pthread_mutex_destroy(&mymutex);
    pthread_cond_destroy(&mycond);

    free(threads);
    return 0;
}