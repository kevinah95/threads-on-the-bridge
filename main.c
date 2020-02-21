#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

#define EAST_DIRECTION 0
#define WEST_DIRECTION 1
#define name(x) ((x==1) ? ("WEST") : ("EAST"))

int east = 0, west = 0;    // # of cars in each direction

sem_t on_the_bridge;

struct car_creation_thread {
    int direction;
    int size;
    double medium;
};

void cross(int direction)
{

    if ((EAST_DIRECTION == direction && west) || (WEST_DIRECTION == direction && east)) {
        if(EAST_DIRECTION==direction)
        {
            printf("waiting [<-] east=%d west=%d thread=%ld\n", east, west, pthread_self());
        } else {
            printf("waiting [->] west=%d east=%d thread=%ld\n", west, east, pthread_self());
        }

        sem_wait(&on_the_bridge);
    }

    if (EAST_DIRECTION == direction)
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

    if (EAST_DIRECTION == direction && 0 < east) {
        --east;
        printf("leave [<-] direction=%s east=%d west=%d thread=%ld\n", name(direction), east, west, pthread_self());
        sleep(1);
        sem_post(&on_the_bridge);
    }

    else if (WEST_DIRECTION == direction && 0 < west) {
        --west;
        printf("leave [->] direction=%s west=%d east=%d thread=%ld\n", name(direction), west, east, pthread_self());
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

void *thread_creation(struct car_creation_thread *car_thread){
    int quantity = 0;
    pthread_t* direction_cars = malloc(sizeof(pthread_t) * car_thread->size);
    while(car_thread->size > 0){
        pthread_create(&direction_cars[quantity], NULL, thread, car_thread->direction);
        quantity--;
        car_thread->size--;
        double random_number = ran_expo(car_thread->medium);
        sleep(random_number);
    }
    for (int i = 0; i < car_thread->size; ++i) {
        pthread_join(direction_cars[i], NULL);
    }
    free(direction_cars);
}

int main(int argc, char *argv[]) {
     int WEST_THREAD_SIZE;
     int EAST_THREAD_SIZE;

    if (argc <= 1) {
      printf("A random number of cars for both directions is set.\n");
      WEST_THREAD_SIZE = rand() % 6;
      EAST_THREAD_SIZE = rand() % 6;
    } else if(argc <= 2) {
      printf("A random number of cars for EAST direction is set.\n");
      WEST_THREAD_SIZE = atoi(argv[1]);
      EAST_THREAD_SIZE = rand() % 6;
    } else {
      WEST_THREAD_SIZE = atoi(argv[1]);
      EAST_THREAD_SIZE  = atoi(argv[2]);
    }

    printf("West cars length: %i\n", WEST_THREAD_SIZE);
    printf("East cars length: %i\n", EAST_THREAD_SIZE);

    sem_init(&on_the_bridge, 0, 1);

    pthread_t* thread_creation_list = malloc(sizeof(pthread_t) * 2);
    //pthread_t east_cars[EAST_THREAD_SIZE], west_cars[WEST_THREAD_SIZE];
    int thread_total = EAST_THREAD_SIZE + WEST_THREAD_SIZE;

    srand(time(NULL));

    struct car_creation_thread thread_weast;
    thread_weast.direction = WEST_DIRECTION;
    thread_weast.size = WEST_THREAD_SIZE;
    thread_weast.medium = 0.01;
    struct car_creation_thread thread_east;
    thread_east.direction = EAST_DIRECTION;
    thread_east.size = EAST_THREAD_SIZE;
    thread_east.medium = 0.07;

    pthread_create(&thread_creation_list[0], NULL, thread_creation, &thread_weast);
    pthread_create(&thread_creation_list[1], NULL, thread_creation, &thread_east);
    pthread_join(thread_creation_list[0], NULL);
    pthread_join(thread_creation_list[1], NULL);

    sem_destroy(&on_the_bridge);
    free(thread_creation_list);
    printf("\n\n\nwest=%d,", west);
    printf("east=%d\n\n\n", east);

    sleep(1);
    exit(0);
}
