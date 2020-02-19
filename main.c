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
            printf("waiting [<-] east=%d thread=%ld\n", east, pthread_self());
        } else {
            printf("waiting [->] west=%d thread=%ld\n", west, pthread_self());
        }

        //sem_wait(&mutex);
        sem_wait(&on_the_bridge);
    }


    if (0 == direction)
    {
        east++;
        printf("(+) [<-] east=%d, thread=%ld\n", east, pthread_self());
    } else {
        west++;
        printf("(+) [->] west=%d, thread=%ld\n", west, pthread_self());
    }

}

void leave(int direction)
{

    if (0 == direction && 0 < east) {

        while(east > 0){
            --east;
            sleep(2);
            printf("leave [<-] direction=%d, thread=%ld\n", direction, pthread_self());
        }
        sem_post(&on_the_bridge);
    }


    else if (1 == direction && 0 < west) {
        while(west > 0){
            --west;
            sleep(2);
            printf("leave [->] direction=%d, thread=%ld\n", direction, pthread_self());
        }
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
    pthread_t t1, t2;

    int iret1, iret2;
    iret1 = pthread_create(&t1, NULL, thread, 0);
    //sleep(2);
    iret2 = pthread_create(&t2, NULL, thread, 1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&on_the_bridge);
    printf("\n\n\nwest=%d,", west);
    printf("east=%d\n\n\n", east);
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);

    sleep(1);
    exit(0);
}

