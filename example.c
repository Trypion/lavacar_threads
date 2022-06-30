// gcc -pthread -o example example.c -lm && ./example

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef WIN32
#include "pthread.h"
#else
#include <pthread.h>
#endif
typedef struct
    thread_data
{
  float a;
  float b;
  float delta;
} tdata_t;
float x[2];

void *computeroot1(void *threadarg)
{
  tdata_t *my_data;
  my_data = (tdata_t *)threadarg;
  x[0] = (-my_data->b + sqrt(my_data->delta)) / (2 * my_data->a);
  pthread_exit(NULL);
}

void *computeroot2(void *threadarg)
{
  tdata_t *my_data;
  my_data = (tdata_t *)threadarg;
  x[1] = (-my_data->b - sqrt(my_data->delta)) / (2 * my_data->a);
  pthread_exit(NULL);
}

int main()
{
  pthread_t tid[2];
  float a, b, c, delta;
  int ret;
  printf("Provide a, b e c\n");
  scanf("%f %f %f", &a, &b, &c);
  delta = b * b - 4 * a * c;
  if (delta < 0)
  {
    printf("Imaginary roots\n");
    exit(0);
  }
  tdata_t data;
  data.a = a;
  data.b = b;
  data.delta = delta;
  ret = pthread_create(&tid[0], NULL, (void *)computeroot1, (void *)&data);
  if (ret < 0)
  {
    printf("Thread creation failed\n");
    exit(0);
  }
  ret = pthread_create(&tid[1], NULL, (void *)computeroot2, (void *)&data);
  if (ret < 0)
  {
    printf("Thread creation failed\n");
    exit(0);
  }
  for (int i = 0; i < 2; i++)
  {
    pthread_join(tid[i], NULL); // Pai espera filho i terminar
    printf("Thread %lu has computed 1st root: %f\n", tid[i], x[i]);
  }
  return 0;
}