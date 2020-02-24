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

//Semaphore pointer
sem_t on_the_bridge, screen;

/*These pthread_cond_t are a wait for the thread to activate
  when a specific condition happens*/
pthread_cond_t condition_west_pass = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition_east_pass = PTHREAD_COND_INITIALIZER;  

//Exclusive value.
pthread_mutex_t lock_bridge = PTHREAD_MUTEX_INITIALIZER;

//Direction value
int bridge_actual_direction = -1;

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
        east++;
        //It rints the name of the process.
        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created [<-] east=%d west=%d thread=%ld\x1b[0m\n", east, west, pthread_self());
        sem_post(&screen);
    } else {
        west++;
        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created [->] west=%d east=%d car thread=%ld\x1b[0m\n", west, east, pthread_self());
        sem_post(&screen);
    }
    //Cars waiting to pass
    int cars_waiting = (direction == EAST_DIRECTION) ? east : west;
    //pthread_mutex_lock(&lock_bridge);
    // The bridge is blocked.
    if(direction == bridge_actual_direction || (bridge_actual_direction == -1 && cars_waiting == 1)){
        
    }else{
        if(EAST_DIRECTION == direction)
        {
            //Cars waiting to pass (east).
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)East car waiting [<-] east=%d west=%d car thread=%ld\x1b[0m\n", east, west, pthread_self());
            sem_post(&screen);
        } else {
            //Cars waiting to pass (west)
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)West car waiting [->] west=%d east=%d thread=%ld\x1b[0m\n", west, east, pthread_self());
            sem_post(&screen);
        }
        //the traffic light waits until the car on the bridge passes.
        sem_wait(&on_the_bridge);
    }
    //The address is updated to the current one.
    bridge_actual_direction = direction;
    sleep(5);
}

void leave(int direction)
{
//if the car is going est
    if (EAST_DIRECTION == direction && 0 < east) {
        --east;
        sem_wait(&screen);
        printf("(--)East car leaving [<-] direction=%s east=%d west=%d thread=%ld\n", name(direction), east, west, pthread_self());
        sem_post(&screen);
    }
//if the car is going est
    else if (WEST_DIRECTION == direction && 0 < west) {
        --west;
        sem_wait(&screen);
        printf("(--)West car leaving [->] direction=%s west=%d east=%d thread=%ld\n", name(direction), west, east, pthread_self());
        sem_post(&screen); 
    }
    //if the bridge is blocked.
    if((east == 0 && bridge_actual_direction == EAST_DIRECTION) || (east == 0 && west > 0)){
        bridge_actual_direction = -1;
    }
    // if the bridge is blocked.
    else if((west == 0 && bridge_actual_direction == WEST_DIRECTION) || (west == 0 && east > 0)){
        bridge_actual_direction = -1;
    }
    //Take out the car that is on the bridge.
    sem_post(&on_the_bridge);
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

    sem_init(&on_the_bridge, 0, 1);
    sem_init(&screen, 0, 1);

    pthread_t* thread_creation_list = malloc(sizeof(pthread_t) * 2);
    //pthread_t east_cars[EAST_THREAD_SIZE], west_cars[WEST_THREAD_SIZE];
    int thread_total = EAST_THREAD_SIZE + WEST_THREAD_SIZE;

    srand(time(NULL));

    //structs definition.
    struct car_creation_thread thread_weast;
    thread_weast.direction = WEST_DIRECTION;
    thread_weast.size = WEST_THREAD_SIZE;
    thread_weast.medium = 0.4;
    struct car_creation_thread thread_east;
    thread_east.direction = EAST_DIRECTION;
    thread_east.size = EAST_THREAD_SIZE;
    thread_east.medium = 0.7;

    pthread_create(&thread_creation_list[0], NULL, thread_creation, &thread_weast);
    pthread_create(&thread_creation_list[1], NULL, thread_creation, &thread_east);
    pthread_join(thread_creation_list[0], NULL);
    pthread_join(thread_creation_list[1], NULL);
    
    printf("\x1b[0m");
    
    sem_destroy(&on_the_bridge);
    sem_destroy(&screen);
    free(thread_creation_list);
    //number of cars that have passed the bridge
    printf("\n\n\ncars that have not passed the bridge yet westbound=%d\n", west);
    printf("cars that have not passed the bridge yet eastbound=%d\n\n\n", east);

    sleep(1);
    exit(0);
}
