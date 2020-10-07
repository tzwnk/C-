//Including needed libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define Ncook 2
#define Noven 5
#define Ndeliverer 10


pthread_mutex_t mymutex;
pthread_cond_t  mycond;


//Functions
void *order(void *threadId);
