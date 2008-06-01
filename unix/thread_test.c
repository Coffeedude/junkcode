#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_m = PTHREAD_COND_INITIALIZER;

int total_sum = 0;
int count = 0;

void thread1_work(int *a)
{
	int i;

	for (i=0; i<*a; i++) {
		usleep(rand() % 1000);
		printf("Thread1: %d\n", i);
		pthread_mutex_lock(&m);
		total_sum += *a;
		pthread_cond_signal(&cond_m);	
		pthread_mutex_unlock(&m);
	}

	return;
}

void thread2_work(int *a)
{
	int i;
	
	for (i=0; i<*a; i++) {		
		usleep(rand() % 1000);
		printf("Thread2: %d\n", i);
		pthread_mutex_lock(&m);
		total_sum += *a;
		pthread_cond_signal(&cond_m);	
		pthread_mutex_unlock(&m);		
	}
	
	return;
	
}

void thread3_work(int *a)
{
	pthread_mutex_lock(&m);
	while (total_sum < count) {
		pthread_cond_wait(&cond_m, &m);
		printf("thread3_work(): Woke up! (%d)\n", total_sum);		
	}
	pthread_mutex_unlock(&m);
	
	printf("Total sum of x*x + y*y = %d\n", total_sum);

	return;	
}

int main(int argc, char *argv[])
{
	pthread_t thread1, thread2, thread3;
	pthread_attr_t pattr;	
	int x, y;
	
	srand(time(NULL));
	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <x> <y>\n", argv[0]);
		return 1;
	}
	
	x = atoi(argv[1]);
	y = atoi(argv[2]);

	count = x*x + y*y;

	pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);
	
	if (pthread_create(&thread1, NULL, (void*)&thread1_work, (void*)&x)) {
		fprintf(stderr, "Error creating thread #1! (%s)\n",
			strerror(errno));
		return -1;
	}
	
	if (pthread_create(&thread2, NULL, (void*)&thread2_work, (void*)&y)) {
		fprintf(stderr, "Error creating thread #2! (%s)\n",
			strerror(errno));
		return -1;
	}

	if (pthread_create(&thread3, NULL, (void*)&thread3_work, NULL)) {
		fprintf(stderr, "Error creating thread #3! (%s)\n",
			strerror(errno));
		return -1;
	}

	pthread_join(thread3, NULL);
	
	return 0;
}
