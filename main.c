//
//  main.c
//  mikeBradyLab2
//
//  Created by Scott McDowell on 10/02/2013.
//  Copyright (c) 2013 Scott McDowell. All rights reserved.
//
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


//#define BLAH 100000000
#define BUFFER_SIZE 10
//#define SEED 3579
pthread_mutex_t mutex; //the mutex lock
sem_t full, empty;//the semaphores
int buffer; //the actual buffer. just add and subtract from the buffer. can implement an item buffer if needed

//pthread_t tid;       //Thread ID
//pthread_attr_t attr; //Set of thread attributes

//void *producer(void *param); /* the producer thread */
//void *consumer(void *param); /* the consumer thread */



void *producer() {
    
    while(1) {
        double rNum = rand() % 4;
        sleep(rNum);
        
        
        sem_wait(&empty); // acquire the empty lock 
        pthread_mutex_lock(&mutex);//acquire the mutex lock */

        
        if(buffer +1 > BUFFER_SIZE ) {
            fprintf(stderr, "Cannot add to buffer\n");
        }
        else while (buffer < BUFFER_SIZE){
            buffer++;
            printf("Produce (buffer : %d)\n", buffer);
        }
        
        pthread_mutex_unlock(&mutex);
        
        sem_post(&full);
    }
}

void *consumer() {
    
    while(1) {
        /* sleep for a random period of time */
        double rNum = rand() % 10;
        sleep(rNum);
        
        sem_wait(&full); /* aquire the full lock */

        pthread_mutex_lock(&mutex);/* aquire the mutex lock */

        if(buffer -1 < 0) {
            fprintf(stderr, "Cannot remove from empty buffer\n");
        }
        else  {
            buffer --;
            printf("Consume (buffer : %d)\n", buffer);
        }
       
        pthread_mutex_unlock(&mutex);
        
        sem_post(&empty);
    }
}


int main() {
    /*Cannot remove from empty buffer
     Produce (buffer : 1)
     Produce (buffer : 2)
     Produce (buffer : 3)
     Produce (buffer : 4)
     Produce (buffer : 5)
     Produce (buffer : 6)
     Consume (buffer : 5)
     Consume (buffer : 4)
     Consume (buffer : 3)
     Consume (buffer : 2)
     Consume (buffer : 1)
     Consume (buffer : 0)
     Produce (buffer : 1)
     Produce (buffer : 2)
     Produce (buffer : 3)
     Produce (buffer : 4)
     Produce (buffer : 5)
     Produce (buffer : 6)
     Cannot add to buffer
     Consume (buffer : 5)
     BYE BYE
*/
    int i;
    int tmp = 10;
    int numProd =1;
    int numCons = 10;
    buffer = 0;

    
    pthread_mutex_init(&mutex, NULL);
    
    sem_init(&empty, 0, 0);/* Create the empty semaphore and initialize to 0 */
    
    sem_init(&full, 0, BUFFER_SIZE);/* Create the full semaphore and initialize to BUFFER_SIZE */

    
    //pthread_attr_init(&attr); //doesnt work without this, ask.
    
    pthread_t threadProd[numProd];
    
    pthread_t threadCons[numCons];
    
    /*create the producer*/
    for(i = 0; i < numProd; i++) {
        pthread_create(&threadProd[i],NULL,producer,NULL);
    }
    
    /* Create the consumer threads */
    for(i = 0; i < numCons; i++) {
        
        pthread_create(&threadCons[i],NULL,consumer,NULL);
    }
    
    /* Sleep for the specified amount of time in milliseconds */
    sleep(tmp);
    
    /* Exit the program */
    printf("BYE BYE\n");
    exit(0);
}