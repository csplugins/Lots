#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>

/*This is usewd to support boolean tpyes in C*/
typedef int bool;
enum {false, true};

/*Semaphore values to aquire possesions safely*/
sem_t agentSem;
sem_t tobacco;
sem_t paper;
sem_t match;
sem_t tobaccoSem;
sem_t paperSem;
sem_t matchSem;
sem_t mutex;
bool isTobacco = false;
bool isPaper = false;
bool isMatch = false;

/*Function protocols to be used later on*/
void* PaperAndMatchAgent();
void* MatchAndTobaccoAgent();
void* TobaccoAndPaperAgent();
void* TobaccoSmoker();
void* PaperSmoker();
void* MatchSmoker();
void* TobaccoPusher();
void* PaperPusher();
void* MatchPusher();

int main(){
  /*Seed the time to get random results every run*/
  srand(time(NULL));
  /*12 pthreads: six smokers, three pushers, and three agents*/
  pthread_t t1, t2, p1, p2, m1, m2, a1, a2, a3, pu1, pu2, pu3;
  /*Initialize the semaphores*/
  /*agentSem and mutex initialized to one, the rest initialized to zero*/
  sem_init(&agentSem, 0, 1);
  sem_init(&tobacco, 0, 0);
  sem_init(&paper, 0, 0);
  sem_init(&match, 0, 0);
  sem_init(&tobaccoSem, 0, 0);
  sem_init(&paperSem, 0, 0);
  sem_init(&matchSem, 0, 0);
  sem_init(&mutex, 0, 1);
  /*Start the pthreads by calling their respective functions*/
  /*These are the agent threads*/
  pthread_create(&a1, NULL, PaperAndMatchAgent, NULL);
  pthread_create(&a2, NULL, MatchAndTobaccoAgent, NULL);
  pthread_create(&a3, NULL, TobaccoAndPaperAgent, NULL);
  /*These are the pusher threads*/
  pthread_create(&pu1, NULL, TobaccoPusher, NULL);
  pthread_create(&pu2, NULL, PaperPusher, NULL);
  pthread_create(&pu3, NULL, MatchPusher, NULL);
  /*These are the smoker threads*/
  pthread_create(&t1, NULL, TobaccoSmoker, NULL);
  pthread_create(&p1, NULL, PaperSmoker, NULL);
  pthread_create(&m1, NULL, MatchSmoker, NULL);
  pthread_create(&t2, NULL, TobaccoSmoker, NULL);
  pthread_create(&p2, NULL, PaperSmoker, NULL);
  pthread_create(&m2, NULL, MatchSmoker, NULL);
  /*Wait for all threads to end before exiting the program*/
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);
  pthread_join(m1, NULL);
  pthread_join(m2, NULL);
  pthread_join(a1, NULL);
  pthread_join(a2, NULL);
  pthread_join(a3, NULL);
  pthread_join(pu1, NULL);
  pthread_join(pu2, NULL);
  pthread_join(pu3, NULL);
  return 0;
}

/*This agent provides paper and match supplies*/
void* PaperAndMatchAgent(){
  int i = 0;
  for(i = 0; i < 6; ++i){
    /*Aquire the agentSem semaphore to continue*/
    sem_wait(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%200) * 1000);
    /*Disply that the agent supplied materials*/
    printf("An agent suplied paper and match.\n");
    /*Release paper and match semaphores*/
    sem_post(&paper);
    sem_post(&match);
  }
}

/*This agent provides match and tobacco supplies*/
void* MatchAndTobaccoAgent(){
  int i = 0;
  for(i = 0; i < 6; ++i){
    /*Aquire the agentSem semaphore to continue*/
    sem_wait(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%200) * 1000);
    /*Disply that the agent supplied materials*/
    printf("An agent supplied match and tobacco.\n");
    /*Release match and tobacco semaphores*/
    sem_post(&match);
    sem_post(&tobacco);
  }
}

/*This agent provides tobacco and paper supplies*/
void* TobaccoAndPaperAgent(){
  int i = 0;
  for(i = 0; i < 6; ++i){
    /*Aquire the agentSem semaphore to continue*/
    sem_wait(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%200) * 1000);
    /*Disply that the agent supplied materials*/
    printf("An agent supplied tobacco and paper.\n");
    /*Release tobacco and paper semaphores*/
    sem_post(&tobacco);
    sem_post(&paper);
  }
}

/*This pusher prevents deadlocks for tobacco*/
void* TobaccoPusher(){
  int i;
  for(i = 0; i < 12; ++i){
    /*Aqure the tobacco semaphore*/
    sem_wait(&tobacco);
    /*Aqure the mutex semaphore*/
    sem_wait(&mutex);
    /*Check the supply conditions*/
    if(isPaper){
      isPaper = false;
      sem_post(&matchSem);
    }
    else if(isMatch){
      isMatch = false;
      sem_post(&paperSem);
    }
    else isTobacco = true;
    /*Release the mutex semaphore*/
    sem_post(&mutex);
  }
}

/*This pusher prevents deadlocks for paper*/
void* PaperPusher(){
  int i;
  for(i = 0; i < 12; ++i){
    /*Aqure the paper semaphore*/
    sem_wait(&paper);
    /*Aqure the mutex semaphore*/
    sem_wait(&mutex);
    /*Check the supply conditions*/
    if(isTobacco){
      isTobacco = false;
      sem_post(&matchSem);
    }
    else if(isMatch){
      isMatch = false;
      sem_post(&tobaccoSem);
    }
    else isPaper = true;
    /*Release the mutex semaphore*/
    sem_post(&mutex);
  }
}

/*This pusher prevents deadlocks for match*/
void* MatchPusher(){
  int i;
  for(i = 0; i < 12; ++i){
    /*Aqure the match semaphore*/
    sem_wait(&match);
    /*Aqure the mutex semaphore*/
    sem_wait(&mutex);
    /*Check the supply conditions*/
    if(isTobacco){
      isTobacco = false;
      sem_post(&paperSem);
    }
    else if(isPaper){
      isPaper = false;
      sem_post(&tobaccoSem);
    }
    else isMatch = true;
    /*Release the mutex semaphore*/
    sem_post(&mutex);
  }
}

/*If the smoker has infinite supply of tobacco*/
void* TobaccoSmoker(){
  int i;
  for(i = 0; i < 3; ++i){
    /*Aquire the tobacco semaphore before continuing*/
    sem_wait(&tobaccoSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is making a cigarette*/
    printf("Tobacco smoker is making a cigarette.\n");
    /*Release the agent semaphore and display to the screen*/
    sem_post(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is smoking a cigarette*/
    printf("Tobacco smoker is smoking a cigarette.\n");
  }
}

/*If the smoker has infinite supply of paper*/
void* PaperSmoker(){
  int i;
  for(i = 0; i < 3; ++i){
    /*Aquire the paper semaphore before continuing*/
    sem_wait(&paperSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is making a cigarette*/
    printf("Paper smoker is making a cigarette.\n");
    /*Release the agent semaphore and display to the screen*/
    sem_post(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is smoking a cigarette*/
    printf("Paper smoker is smoking a cigarette.\n");
  }
}

/*If the smoker has infinite supply of matches*/
void* MatchSmoker(){
  int i;
  for(i = 0; i < 3; ++i){
    /*Aquire the paper semaphore before continuing*/
    sem_wait(&matchSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is making a cigarette*/
    printf("Match smoker is making a cigarette.\n");
    /*Release the agent semaphore and display to the screen*/
    sem_post(&agentSem);
    /*usleep will sleep for microseconds multiplied by 1000 to get miliseconds*/
    usleep((rand()%50) * 1000);
    /*Display that the smoker is smoking a cigarette*/
    printf("Match smoker is smoking a cigarette.\n");
  }
}
