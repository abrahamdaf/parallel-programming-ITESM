#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define points 10000

int pointsInCircle = 0;
int pointsToMake = points;

pthread_mutex_t lock;

void* monteCarloEstimation(void* arg){
	int localCircleCount = 0;
	int iterations = (int) arg;
	double x,y;
	for(int i = 0; i < iterations; i++){

		x = (double)(rand()%10000)/10000;
		y = (double)(rand()%10000)/10000;
		if((x*x + y*y)<1){
			localCircleCount++;
		
		}
		

	}
	pthread_mutex_lock(&lock);
	pointsInCircle += localCircleCount;
	pthread_mutex_unlock(&lock);

	pthread_exit(NULL);
}

int main(int argc, char* argv[]){
	
	int threadNum = atoi(argv[1]);
		

	pthread_t threads[threadNum];

	int checkThreadCreation;

	if (pthread_mutex_init(&lock, NULL) != 0){
		printf("\n mutex init failed\n");
		return 1;

	}

	
	int i = 0;
	int err;

	while(i < threadNum) {
        	err = pthread_create(&(threads[i]), NULL, &monteCarloEstimation, (void*) (points/threadNum));
            	if (err != 0)
                	printf("\ncan't create thread :[%s]", strerror(err));
                i++;
    	}

	for (int j = 0; j < threadNum; j++){

		pthread_join(threads[j], NULL);

	}

	

	pthread_mutex_destroy(&lock);
	double pi = (double)4*pointsInCircle/points;

	printf("Calculated value of pi = %f \n", pi);
	printf("C: %d\nS: %d\n",pointsInCircle,points);
	return 0;
}
