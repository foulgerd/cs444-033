#include<pthread.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define PHILOSOPHERS 5

pthread_mutex_t fork_wall;
int forks[PHILOSOPHERS];

int eat() {
        int wait;
        wait = rand()%7 + 2;
        sleep(wait);
}

int think() {
        int wait;
        wait = rand()%19 + 1;
        sleep(wait);
}

void dinner(int i) {
        while(1) {
                //pthread_mutex_lock(&fork_wall);
                if (forks[i] == 0 || forks[(i+1) % PHILOSOPHERS] == 0) {
                        pthread_mutex_lock(&fork_wall);
                        forks[i] = 1;                       //left fork
                        forks[(i+1) % PHILOSOPHERS] = 1;    //right fork
                        printf("Philosopher #%d is eating.\n", i);
                        eat();
                        pthread_mutex_unlock(&fork_wall);
                }

                pthread_mutex_lock(&fork_wall);
                forks[i] = 0;
                forks[(i+1) % PHILOSOPHERS] = 0;
                pthread_mutex_unlock(&fork_wall);
                printf("Philosopher #%d is thinking.\n", i);
                think();
        }
}

int main() {
        pthread_t philosophers[PHILOSOPHERS];
        int i;
        char name_temp[6];
        srand(time(NULL));

        for (i = 0; i < PHILOSOPHERS; i++) {
                forks[i] = 0;
        }

        //init the fork mutex
        if (pthread_mutex_init(&fork_wall, NULL)) {
                printf("Error initializing mutex.\n");
                exit(0);
        } 

        for (i = 0; i < PHILOSOPHERS; i++) {
                pthread_create(&philosophers[i], NULL, (void *)dinner, (void *) i);
        }

        for (i = 0; i < PHILOSOPHERS; i++) {
                pthread_join(philosophers[i], NULL);
        }

        pthread_mutex_destroy(&fork_wall);
        pthread_exit(0);

        return 0;
}



