#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep(). man 3 sleep for details.
#include <pthread.h>
#include <semaphore.h>

int east = 0, west = 0;    // # of cars in each direction

sem_t mutex;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void cross(int direction)
{

    // wait for any cars already going in opposite direction
    if ((0 == direction && west) || (1 == direction && east)) {
        //busy.wait;
        if(0==direction)
        {
            printf("waiting [<-] east=%d thread=%ld\n", east, pthread_self());
        } else {
            printf("waiting [->] east=%d thread=%ld\n", west, pthread_self());
        }

        //sem_wait(&mutex);
        pthread_mutex_lock( &mutex1 );
        //printf("cross | direction=%d thread=%ld\n", direction, pthread_self());
    }


    // now increment counter of cars heading in my direction
    if (0 == direction)    // NB: 'else' is not appropriate here!
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

    // if we're the last car off heading east,
    // signal all west traffic that it's ok to proceed
    if (0 == direction && 0 == --east) {
        //while (busy.queue) busy.signal;
        sleep(2);
        printf("leave [<-] direction=%d, thread=%ld\n", direction, pthread_self());
        //sem_post(&mutex);
        pthread_mutex_unlock( &mutex1 );
    }


        // if we're the last car off heading west,
        // signal all east traffic that it's ok to proceed
    else if (1 == direction && 0 == --west) {
        //while (busy.queue) busy.signal;
        sleep(2);
        printf("leave [->] direction=%d, thread=%ld\n", direction, pthread_self());
        //sem_post(&mutex);
        pthread_mutex_unlock( &mutex1 );
    }

}

void *thread(int *direction)
{
    cross(direction);
    leave(direction);
    cross(direction);
    leave(direction);
}

int main() {

    sem_init(&mutex, 0, 2);
    pthread_t t1, t2;

    int iret1, iret2;
    iret1 = pthread_create(&t1, NULL, thread, 0);
    //sleep(2);
    iret2 = pthread_create(&t2, NULL, thread, 1);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    sem_destroy(&mutex);
    printf("\n\n\nwest=%d,", west);
    printf("east=%d\n\n\n", east);
    printf("Thread 1 returns: %d\n",iret1);
    printf("Thread 2 returns: %d\n",iret2);

    sleep(1);
    exit(0);
}

