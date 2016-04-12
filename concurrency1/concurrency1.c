#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

#define BUFFER_MAX 32

pthread_mutex_t excluder;
pthread_cond_t p, c;

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

int checkFinished(int i) {
        int check = 0;

        if (buffer[i].wait == 0 || buffer[i].number == 33) {
                check = 1;
        }

        return check;
}

void produce(){
        int i = 0; 
        int cont = 0;
        
        pthread_mutex_trylock(&excluder);
        while(i != 32) {
                //for (i = 0; i < BUFFER_MAX; i++) {
                        if (checkFinished(i)) {
                                buffer[i].number = randomNumbers(1);
                                buffer[i].wait = randomNumbers(3);
                                sleep(buffer[i].wait);
                                i++;
                        }
                //}
        }
        pthread_mutex_unlock(&excluder);
}

void consume(){
        int i; 
        int cont = 0;

        pthread_mutex_trylock(&excluder);
        while(i != 32) {
                //for (i = 0; i < BUFFER_MAX; i++) {
                        if (!checkFinished(i)) {
                                printf("Printing number: %d\n", buffer[i].number);
                                buffer[i].wait = randomNumbers(2);
                                sleep(buffer[i].wait);
                                buffer[i].number = 33;
                                buffer[i].wait = 0;
                                i++;
                        }
                //}
        }
        pthread_mutex_unlock(&excluder);
}

int main(int argc, char** argv) {
        pthread_t producer, consumer;
        int i;

        if (pthread_mutex_init(&excluder, NULL)) {
                printf("Error initializing mutex.\n");
                exit(0);
        }

        if (pthread_cond_init(&p, NULL)) {
                printf("Error initializing producer condition.\n");
                exit(0);
        }

        if (pthread_cond_init(&c, NULL)) {
                printf("Error initializing consumer condition.\n");
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
        
        pthread_join(producer, NULL);
        pthread_join(consumer, NULL);

        pthread_mutex_destroy(&excluder);
        pthread_exit(0);

        return 0;
}

