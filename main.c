#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

/*DDefinition of constants. Cars to the east will have the traffic light on 0
cars to the west will have the traffic light on 1*/
#define EAST_DIRECTION 0
#define WEST_DIRECTION 1

/*the name function will recieve a number x
  if x ==0, the direction will be west, if not,
  the direction will be east*/
#define name(x) ((x==1) ? ("WEST") : ("EAST"))


int east = 0, west = 0;    // # of cars in each direction
int east_pass = 0, west_pass = 0;    // # of cars in each direction
int bridge_direction = -1;    // # of cars in each direction

//Semaphore pointer
sem_t sem_east, sem_west, screen, sem_order, sem_bridge;

//Cars thread struct.
struct car_creation_thread {
    int direction;
    int size;
    double medium;
};

//Cross method will recieve est or west
void cross(int direction)
{   
    if (EAST_DIRECTION == direction)
    {   
        sem_wait(&sem_order);
        east++;
        sem_post(&sem_order);

        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created (EAST) [<-] east=%d west=%d thread=%ld\x1b[0m\n", east, west, pthread_self());
        sem_post(&screen);
    } else {
        sem_wait(&sem_order);
        west++;
        sem_post(&sem_order);
        
        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created (WEST) [->] west=%d east=%d thread=%ld\x1b[0m\n", west, east, pthread_self());
        sem_post(&screen);
    }
    
    //Time to arrive in the bridge
    sleep(5);

    if(EAST_DIRECTION == direction)
        {
            //Cars waiting to pass (east).
            sem_wait(&sem_order);
            east_pass++;

            sem_wait(&screen);
            printf("\x1b[0;34m\x1b[1m(...)East car arrive to the bridge [<-] east=%d west=%d east_pass=%d thread=%ld\x1b[0m\n", east, west, east_pass, pthread_self());
            sem_post(&screen);

            sem_post(&sem_order);

            if (bridge_direction == WEST_DIRECTION){
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)East waiting [<-] east_pass=%d  thread=%ld\x1b[0m\n", east_pass, pthread_self());
                sem_post(&screen);
            }


            sem_wait(&sem_east);

            sem_wait(&sem_bridge);
            if (bridge_direction == EAST_DIRECTION){
                
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)East (1) passing the bridge [<-] east_pass=%d thread=%ld\x1b[0m\n", east_pass, pthread_self());
                sem_post(&screen);
            
            } else if (bridge_direction == -1 || east_pass == 1){
                sem_wait(&sem_west);
                bridge_direction = EAST_DIRECTION;
                
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)East start to pass [<-] east_pass=%d  thread=%ld\x1b[0m\n", east_pass, pthread_self());
                sem_post(&screen);   
            }
            sem_post(&sem_bridge);

        } else {
            //Cars waiting to pass (west)
            sem_wait(&sem_order);
            west_pass++;
            sem_wait(&screen);
            printf("\x1b[0;34m\x1b[1m(...)West car arrive to the bridge [->] west=%d east=%d west_pass=%d thread=%ld\x1b[0m\n", west, east,west_pass, pthread_self());
            sem_post(&screen);
            sem_post(&sem_order);

            if (bridge_direction == EAST_DIRECTION){
            
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)West waiting [->] west_pass=%d  thread=%ld\x1b[0m\n", west_pass, pthread_self());
                sem_post(&screen);
            
            }


            sem_wait(&sem_west);
            
            sem_wait(&sem_bridge);
            if (bridge_direction == WEST_DIRECTION){
             
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)West (1) passing the bridge [->] west_pass=%d thread=%ld\x1b[0m\n", west_pass, pthread_self());
                sem_post(&screen);
            
            } else if (bridge_direction == -1 || west_pass == 1){
            
                sem_wait(&sem_east);
                bridge_direction = WEST_DIRECTION;
                sem_wait(&screen);
                printf("\x1b[31;1m\x1b[1m(...)West start to pass [->] west_pass=%d  thread=%ld\x1b[0m\n", west_pass, pthread_self());
                sem_post(&screen);
            
            }
            
            sem_post(&sem_bridge);
        }
}

void leave(int direction)
{
//Time in the bridge
sleep(2);

//if the car is going est
    if (EAST_DIRECTION == direction) {
        sem_wait(&screen);
        printf("\x1b[0;33m\x1b[1m(--)East car leave [<-] direction=%s east=%d west=%d thread=%ld\x1b[0m\n", name(direction), east, west, pthread_self());
        sem_post(&screen);
        sem_post(&sem_east);

        sem_wait(&sem_order);
        --east;
        --east_pass;
        if (east_pass == 0){
            sem_post(&sem_west);
            bridge_direction = -1;
        }
        sem_post(&sem_order);
    }
//if the car is going est
    else if (WEST_DIRECTION == direction) {
        sem_wait(&screen);
        printf("\x1b[0;33m\x1b[1m(--)West car leave [->] direction=%s east=%d west=%d thread=%ld\x1b[0m\n", name(direction), east, west, pthread_self());
        sem_post(&screen);
        sem_post(&sem_west);

        sem_wait(&sem_order);
        --west;
        --west_pass;
        if (west_pass == 0){
            sem_post(&sem_east);
            bridge_direction = -1;
        }
        sem_post(&sem_order);
    }
}

//The thread is going to be a pointer to the address.
void *thread(int *direction)
{
    cross(direction);
    leave(direction);
}

//Random number for not declared directiion.
double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

//Thread creation receives a pointer to the car_thread scruct
void *thread_creation(struct car_creation_thread *car_thread){
    int quantity = 0;

    //The car address will be the size of the car size.
    pthread_t* direction_cars = malloc(sizeof(pthread_t) * car_thread->size);

    while(car_thread->size > 0){
        /*Whenever the number of cars is greater than 0, a thread is created
        with the number of cars and their address*/
        pthread_create(&direction_cars[quantity], NULL, thread, car_thread->direction);
        //Increase the number of cars that have passed the bridge.
        quantity++;
        //decrease the pile of cars to pass.
        car_thread->size--;
        double random_number = ran_expo(car_thread->medium);
        sleep(random_number);
    }
    //Threads synchronization.
    for (int i = 0; i < quantity; ++i) {
        pthread_join(direction_cars[i], NULL);
    }
    //clean the direction the cars are heading.
    free(direction_cars);
}

int main(int argc, char *argv[]) {
     int WEST_THREAD_SIZE;
     int EAST_THREAD_SIZE;

     /*If the number of arguments entered is less than or equal to 1 then,
     a number of random cars is defined in both directions*/
    if (argc <= 1) {
      printf("A random number of cars for both directions is set.\n");
      WEST_THREAD_SIZE = rand() % 6;
      EAST_THREAD_SIZE = rand() % 6;
      //east random number definition.
    } else if(argc <= 2) {
      printf("A random number of cars for heading east is set.\n");
      WEST_THREAD_SIZE = atoi(argv[1]);
      EAST_THREAD_SIZE = rand() % 6;
    } else {
      //conversion of received arguments.
      WEST_THREAD_SIZE = atoi(argv[1]);
      EAST_THREAD_SIZE  = atoi(argv[2]);
    }

    printf("Quantity of cars heading west: %i\n", WEST_THREAD_SIZE);
    printf("Quantity of cars heading east: %i\n", EAST_THREAD_SIZE);

    sem_init(&sem_order, 0, 1);
    sem_init(&sem_bridge, 0, 1);
    sem_init(&sem_east, 0, 1);
    sem_init(&sem_west, 0, 1);
    sem_init(&screen, 0, 1);

    pthread_t* thread_creation_list = malloc(sizeof(pthread_t) * 2);
    int thread_total = EAST_THREAD_SIZE + WEST_THREAD_SIZE;

    srand(time(NULL));

    //structs definition.
    struct car_creation_thread thread_west;
    thread_west.direction = WEST_DIRECTION;
    thread_west.size = WEST_THREAD_SIZE;
    thread_west.medium = 0.4;
    struct car_creation_thread thread_east;
    thread_east.direction = EAST_DIRECTION;
    thread_east.size = EAST_THREAD_SIZE;
    thread_east.medium = 0.7;

    pthread_create(&thread_creation_list[0], NULL, thread_creation, &thread_west);
    pthread_create(&thread_creation_list[1], NULL, thread_creation, &thread_east);
    pthread_join(thread_creation_list[0], NULL);
    pthread_join(thread_creation_list[1], NULL);
    
    printf("\x1b[0m");
    
    sem_destroy(&screen);
    sem_destroy(&sem_order);
    sem_destroy(&sem_west);
    sem_destroy(&sem_bridge);
    sem_destroy(&sem_east);
    free(thread_creation_list);

    sleep(1);
    exit(0);
}
