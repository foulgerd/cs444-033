#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_MAX 32

pthread_mutex_t excluder;

struct Buffer{
        int number;
        int wait;
};

struct Buffer buffer[BUFFER_MAX];

int randomNumbers(int pc){
        int random;
        switch(pc) { 
                case 1:                 //Producer random number
                        random = (rand() % 10);
                        break;
                case 2:                 //Consumer random wait time
                        random = (rand() % 8) + 2;
                        break;
                case 3:                 //Producer random wait time
                        random = (rand() % 5) + 3;
                        break;
        }

        return random;
}

void produce(){
        int i;
        for (i = 0; i < BUFFER_MAX; i++) {
                pthread_mutex_lock(&excluder);
                buffer[i].number = randomNumbers(1);
                buffer[i].wait = randomNumbers(3);
        }
        pthread_mutex_unlock(&excluder);
}

void consume(){
        int i;
        for (i = 0; i < BUFFER_MAX; i++) {
                pthread_mutx_lock(&excluder);
        }
}

int main(int argc, char** argv) {
        pthread_t producer, consumer;

        if (pthread_mutex_init(&excluder, NULL)) {
                printf("Error initializing mutex.\n");
                exit(0);
        }
        
        if (pthread_create(&producer, NULL, (void *)produce, NULL)) {
                printf("Error creating producer thread.\n");
                exit(0);
        }
        
        if (pthread_create(&consumer, NULL, (void *)consume, NULL)) {
                printf("Error creating consumer thread.\n");
                exit(0);
        }

        return 0;
}

