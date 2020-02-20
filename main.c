#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define EAST_THREAD_SIZE 2
#define WEST_THREAD_SIZE 2
#define EAST_DIRECTION 0
#define WEST_DIRECTION 1

int east = 0, west = 0;    // # of cars in each direction

sem_t on_the_bridge;

void cross(int direction)
{

    if ((0 == direction && west) || (1 == direction && east)) {
        if(0==direction)
        {
            printf("waiting [<-] east=%d west=%d thread=%ld\n", east, west, pthread_self());
        } else {
            printf("waiting [->] west=%d east=%d thread=%ld\n", west, east, pthread_self());
        }

        sem_wait(&on_the_bridge);
    }


    if (0 == direction)
    {
        east++;
        printf("(+) [<-] east=%d west=%d thread=%ld\n", east, west, pthread_self());
    } else {
        west++;
        printf("(+) [->] west=%d east=%d thread=%ld\n", west, east, pthread_self());
    }

}

void leave(int direction)
{

    if (0 == direction && 0 < east) {

        --east;
        printf("leave [<-] direction=%d east=%d west=%d thread=%ld\n", direction, east, west, pthread_self());
        sleep(1);
        sem_post(&on_the_bridge);
    }


    else if (1 == direction && 0 < west) {
        --west;
        printf("leave [->] direction=%d west=%d east=%d thread=%ld\n", direction, west, east, pthread_self());
        sleep(1);
        sem_post(&on_the_bridge);
    }

}

void *thread(int *direction)
{
    cross(direction);
    leave(direction);
}

double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

int main() {

    sem_init(&on_the_bridge, 0, 1);
    pthread_t east_cars[EAST_THREAD_SIZE], west_cars[WEST_THREAD_SIZE];
    int thread_total = EAST_THREAD_SIZE + WEST_THREAD_SIZE;
    int quantity_of_east = 0;
    int quantity_of_west = 0;
    srand(time(NULL));
    while(thread_total != 0){
        pthread_create(&east_cars[quantity_of_east], NULL, thread, EAST_DIRECTION);
        quantity_of_east++;
        thread_total--;
        int random_number = (int)ran_expo(0.7);
        printf("random=%d", random_number);
        sleep(random_number);
        pthread_create(&west_cars[quantity_of_west], NULL, thread, WEST_DIRECTION);
        quantity_of_west++;
        thread_total--;
    }

    for (int i = 0; i < EAST_THREAD_SIZE; ++i) {
        pthread_join(east_cars[i], NULL);
    }

    for (int j = 0; j < WEST_THREAD_SIZE; ++j) {
        pthread_join(west_cars[j], NULL);
    }

    sem_destroy(&on_the_bridge);
    printf("\n\n\nwest=%d,", west);
    printf("east=%d\n\n\n", east);

    sleep(1);
    exit(0);
}

