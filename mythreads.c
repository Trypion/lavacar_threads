// gcc -pthread -o mythreads mythreads.c && ./mythreads

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#ifdef WIN32
#include "pthread.h"
#else
#include <pthread.h>
#endif

void *func1(void *arg);
void *func2(void *arg);

int main()
{
  pthread_t thid1, thid2;
  void *ret;
  char *ptr_char = "thread 2";

  if (pthread_create(&thid1, NULL, func1, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid2, NULL, func2, (void *)ptr_char) != 0)
    exit(1);

  if (pthread_join(thid1, &ret) != 0)
    exit(3);

  printf("thread exited with: %s \n", (char *)ret);
  if (pthread_join(thid2, NULL) != 0)
    exit(3);

  pthread_exit(NULL);
}

void *func1(void *arg)
{
  char *ret;
  printf("thread %lu entered with argument %s\n", pthread_self(), (char *)arg);
  if ((ret = (char *)malloc(20)) == NULL)
  {
    perror("malloc() error");
    exit(2);
  }
  strcpy(ret, "This is a test");
  pthread_exit(ret);
}

void *func2(void *arg)
{
  sleep(2);
  printf("thread %lu entered with argument %s\n", pthread_self(), (char *)arg);
  pthread_exit(NULL);
}