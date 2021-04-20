#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int eastBaboons = 0;
int westBaboons = 0;

sem_t rope;
sem_t turnstyle;
sem_t eastMutex;
sem_t westMutex;
sem_t maxBaboons;

void *eastCrossing(void *arg)
{
    int t_id = *((int *)arg);
    printf("Baboon initialized and wanting to go EAST with id %d \n", t_id);

    sem_wait(&turnstyle);
    sem_wait(&eastMutex);
    eastBaboons++;
    usleep(1);
    printf("Baboon ready to climb the rope to the EAST with id %d \n", t_id);

    if (eastBaboons == 1)
    {
        sem_wait(&rope);
    }

    sem_post(&eastMutex);
    sem_post(&turnstyle);

    sem_wait(&maxBaboons);
    printf("Baboon crossing the rope to the EAST with id %d \n", t_id);
    usleep(1);
    sem_post(&maxBaboons);

    sem_wait(&eastMutex);
    eastBaboons--;

    if (eastBaboons == 0)
    {
        sem_post(&rope);
    }

    sem_post(&eastMutex);
    printf("Baboon crossed the rope to the EAST with id %d \n", t_id);

    return 0;
}

void *westCrossing(void *arg)
{
    int t_id = *((int *)arg);
    printf("Baboon initialized and wanting to go WEST with id %d \n", t_id);

    sem_wait(&turnstyle);
    sem_wait(&westMutex);
    westBaboons++;
    usleep(1);
    printf("Baboon ready to climb the rope to the WEST with id %d \n", t_id);

    if (westBaboons == 1)
    {
        sem_wait(&rope);
    }

    sem_post(&westMutex);
    sem_post(&turnstyle);

    sem_wait(&maxBaboons);
    printf("Baboon crossing the rope to the WEST with id %d \n", t_id);
    usleep(1);
    sem_post(&maxBaboons);

    sem_wait(&westMutex);
    westBaboons--;

    if (westBaboons == 0)
    {
        sem_post(&rope);
    }

    sem_post(&westMutex);
    printf("Baboon crossed the rope to the WEST with id %d \n", t_id);

    return 0;
}

int main(int argc, char **argv)
{
    int n_threads = atoi(argv[1]);
    int ids[n_threads];
    int i;
    int direction;
    pthread_t threads[n_threads];

    srand(time(NULL));

    sem_init(&rope, 0, 1);
    sem_init(&turnstyle, 0, 1);
    sem_init(&eastMutex, 0, 1);
    sem_init(&westMutex, 0, 1);
    sem_init(&maxBaboons, 0, 5);

    for (i = 0; i < n_threads; i++)
    {
        direction = rand() % 2;
        ids[i] = i;
        pthread_create(&threads[i], NULL, direction == 0 ? eastCrossing : westCrossing, &ids[i]);
    }

    for (i = 0; i < n_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&rope);
    sem_destroy(&turnstyle);
    sem_destroy(&eastMutex);
    sem_destroy(&westMutex);
    sem_destroy(&maxBaboons);

    return 0;
}
