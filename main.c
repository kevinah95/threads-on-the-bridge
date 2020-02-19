#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <semaphore.h>

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
        //sleep(10);
        sem_post(&on_the_bridge);
    }


    else if (1 == direction && 0 < west) {
        --west;
        printf("leave [->] direction=%d west=%d east=%d thread=%ld\n", direction, west, east, pthread_self());
        //sleep(10);
        sem_post(&on_the_bridge);
    }

}

void *thread(int *direction)
{
    cross(direction);
    leave(direction);
}

int main() {

    sem_init(&on_the_bridge, 0, 1);
    pthread_t t1, t2, t3, t4;

    int iret1, iret2, iret3, iret4;
    iret1 = pthread_create(&t1, NULL, thread, 0);
    iret2 = pthread_create(&t2, NULL, thread, 1);

    iret3 = pthread_create(&t3, NULL, thread, 0);
    iret4 = pthread_create(&t4, NULL, thread, 1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    sem_destroy(&on_the_bridge);
    printf("\n\n\nwest=%d,", west);
    printf("east=%d\n\n\n", east);
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);
    printf("Thread 3 returns: %d\n",iret3);
    printf("Thread 4 returns: %d\n",iret4);

    sleep(1);
    exit(0);
}

