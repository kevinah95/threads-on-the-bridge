#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>

/*Definition of constants. Cars to the east will have the traffic light on 0
cars to the west will have the traffic light on 1*/
#define EAST_DIRECTION 0
#define WEST_DIRECTION 1

/*the name function will recieve a number x
  if x ==0, the direction will be west, if not,
  the direction will be east*/
#define name(x) ((x==1) ? ("WEST") : ("EAST"))


int east = 0, west = 0;    // # of cars in each direction
int east_pass = 0, west_pass = 0;    // # of cars from each direction on the bridge
int bridge_direction = -1;    // # of cars in each direction

pthread_mutex_t mut_east = PTHREAD_MUTEX_INITIALIZER; //Semaphore to stop cars from east
pthread_mutex_t mut_west = PTHREAD_MUTEX_INITIALIZER; //Semaphore to stop cars from west

pthread_cond_t con_east = PTHREAD_COND_INITIALIZER; //Condition to broadcast east side
pthread_cond_t con_west = PTHREAD_COND_INITIALIZER; //Condition to broadcast west side


//Semaphore pointer
sem_t sem_real_bridge, screen, sem_order;
//The first one is to see if the brige is free
//Second is for print
//Third one is for manage the conditional variables


//Cars thread struct.
struct car_creation_thread {
    int direction;
    int size;
    double medium;
};

//Cross method will recieve est or west
void cross(int direction)
{   
    int actual_east_pass = 0;
    int actual_west_pass = 0;

    if (EAST_DIRECTION == direction)
    {   
        //East car created
        sem_wait(&sem_order);
        east++;
        sem_post(&sem_order);

        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created (EAST) [<-] east=%d west=%d thread=%ld\x1b[0m\n", east, west, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
    } else {
        //West car created
        sem_wait(&sem_order);
        west++;
        sem_post(&sem_order);
        
        sem_wait(&screen);
        printf("\x1b[32;1m\x1b[1m(+) Cars created (WEST) [->] west=%d east=%d thread=%ld\x1b[0m\n", west, east, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
    }
    
    //Time to arrive in the bridge
    sleep(5);

    if(EAST_DIRECTION == direction)
    {
        //We wait for the cars to arrive into the bridge
        sem_wait(&sem_order);
        east_pass++;
        actual_east_pass = east_pass;
        sem_wait(&screen);
        printf("\x1b[0;34m\x1b[1m(...)East car arrive to the bridge [<-] east=%d west=%d east_pass=%d thread=%ld\x1b[0m\n", east, west, east_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
        sem_post(&sem_order);

        if (bridge_direction == WEST_DIRECTION){ //Direction of the brige is in the opposite
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)East waiting [<-] east_pass=%d  thread=%ld\x1b[0m\n", east_pass, pthread_self());
            fflush(stdin); 
            sem_post(&screen);
        }

        if (bridge_direction != EAST_DIRECTION && actual_east_pass == 1){
            //Opposite direction free, I'm the first

            sem_wait(&screen);
            fflush(stdin); 
            sem_post(&screen);   

            sem_wait(&sem_real_bridge);
            bridge_direction = EAST_DIRECTION;
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)East start to pass [<-] east_pass=%d  thread=%ld\x1b[0m\n", east_pass, pthread_self());
            fflush(stdin); 
            sem_post(&screen);   
            pthread_cond_broadcast(&con_east);

        }else if (bridge_direction != EAST_DIRECTION){ //Opposite direction and is not the fisrt
            pthread_mutex_lock(&mut_east);
            pthread_cond_wait(&con_east, &mut_east);//We wait for the first one to move on
            pthread_mutex_unlock(&mut_east);
        }

        sem_wait(&screen);
        printf("\x1b[31;1m\x1b[1m(...)East passing the bridge [<-] east_pass=%d thread=%ld\x1b[0m\n", east_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);

    } else {
        //(WEST)
        //We wait for the cars to arrive into the bridge
        sem_wait(&sem_order);
        west_pass++;
        actual_west_pass =  west_pass;
        sem_wait(&screen);
        printf("\x1b[0;34m\x1b[1m(...)West car arrive to the bridge [->] west=%d east=%d west_pass=%d thread=%ld\x1b[0m\n", west, east,west_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
        sem_post(&sem_order);

        if (bridge_direction == EAST_DIRECTION){ //Direction of the brige is in the opposite
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)West waiting [->] west_pass=%d  thread=%ld\x1b[0m\n", west_pass, pthread_self());
            fflush(stdin); 
            sem_post(&screen);
        }
        
        if (bridge_direction != WEST_DIRECTION && actual_west_pass == 1){
            //Opposite direction free, I'm the first

            sem_wait(&screen);
            fflush(stdin); 
            sem_post(&screen);   

            sem_wait(&sem_real_bridge);
            bridge_direction = WEST_DIRECTION;
            sem_wait(&screen);
            printf("\x1b[31;1m\x1b[1m(...)West start to pass [->] west_pass=%d  thread=%ld\x1b[0m\n", west_pass, pthread_self());
            fflush(stdin); 
            sem_post(&screen);
            pthread_cond_broadcast(&con_west);

        }else if (bridge_direction != WEST_DIRECTION){ //Opposite direction and is not the fisrt
            pthread_mutex_lock(&mut_west);
            pthread_cond_wait(&con_west, &mut_west);
            pthread_mutex_unlock(&mut_west);
        }

        sem_wait(&screen);
        printf("\x1b[31;1m\x1b[1m(...)West passing the bridge [->] west_pass=%d thread=%ld\x1b[0m\n", west_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
    }
}

void leave(int direction)
{
//Time inside the bridge
sleep(2);

    //Bridge is on east side
    if (EAST_DIRECTION == direction) {
        sem_wait(&sem_order);
        east--;
        east_pass--;
        sem_wait(&screen);
        printf("\x1b[0;33m\x1b[1m(--)East car leave [<-] direction=%s east=%d west=%d east_pass=%d thread=%ld\x1b[0m\n", name(direction), east, west,east_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
        if (east_pass == 0){
            sem_post(&sem_real_bridge);
            bridge_direction = -1;
        }else{
            pthread_cond_broadcast(&con_east);
        }
        sem_post(&sem_order);
    }
    //Bridge is on west side
    else if (WEST_DIRECTION == direction) {
        sem_wait(&sem_order);
        west--;
        west_pass--;
        sem_wait(&screen);
        printf("\x1b[0;33m\x1b[1m(--)West car leave [->] direction=%s east=%d west=%d west_pass=%d thread=%ld\x1b[0m\n", name(direction), east, west,west_pass, pthread_self());
        fflush(stdin); 
        sem_post(&screen);
        if (west_pass == 0){
            sem_post(&sem_real_bridge);
            bridge_direction = -1;
        }else{
            pthread_cond_broadcast(&con_west);
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
  sem_init(&sem_real_bridge, 0, 1);
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
  sem_destroy(&sem_real_bridge);
  free(thread_creation_list);

  pthread_mutex_destroy(&mut_west);
  pthread_mutex_destroy(&mut_east);

  pthread_cond_destroy(&con_west);
  pthread_cond_destroy(&con_east);  

  sleep(1);
  exit(0);
}
