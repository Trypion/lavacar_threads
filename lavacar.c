// gcc -pthread -o lavacar lavacar.c && ./lavacar

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#ifdef WIN32
#include "pthread.h"
#else
#include <pthread.h>
#endif

pthread_mutex_t lock;
sem_t sem;

void *funcionario(void *arg);
void *loja(void *arg);

int num_vagas = 4;
int num_clientes = 13;

// ========================================================
struct Node
{
  int data;
  struct Node *next;
};

typedef struct Node node;
int size = 0;

node *front = NULL;
node *rear = NULL;
// ========================================================

void Enqueue(int n)
{

  node *new_node = (node *)malloc(sizeof(node));

  new_node->data = n;
  new_node->next = NULL;

  if (front == NULL && rear == NULL)
  {
    front = new_node;
    rear = new_node;
    return;
  }

  rear->next = new_node;
  rear = new_node;
}

void Dequeue()
{

  node *tmp = front;

  if (front == rear)
  {
    front = rear = NULL;
  }

  else
  {
    front = front->next;
  }
  free(tmp);
}

void Show()
{

  node *tmp = front;

  while (tmp != NULL)
  {

    printf("%d \n", tmp->data);
    tmp = tmp->next;
  }
}

int main()
{
  pthread_t thid1, thid2, thid3, thid4;

  char *ptr_char = "thread 2";

  // iniciar o semaforo deixando 2 threads acessar a região crítica
  sem_init(&sem, 0, 2);
  pthread_mutex_init(&lock, NULL);

  if (pthread_create(&thid1, NULL, funcionario, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid3, NULL, funcionario, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid4, NULL, funcionario, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid2, NULL, loja, (void *)ptr_char) != 0)
    exit(1);

  if (pthread_join(thid1, NULL) != 0)
    exit(3);

  if (pthread_join(thid2, NULL) != 0)
    exit(3);

  if (pthread_join(thid3, NULL) != 0)
    exit(3);

  if (pthread_join(thid4, NULL) != 0)
    exit(3);

  pthread_mutex_destroy(&lock);
  sem_destroy(&sem);

  pthread_exit(NULL);
}

void *funcionario(void *arg)
{
  while (1)
  {
    sem_wait(&sem);
    pthread_mutex_lock(&lock);
    if (front != NULL)
    {
      int cliente = front->data;
      printf("func: %ld -> lavando carro do cliente: %d \n", (long) pthread_self(), cliente);
      Dequeue();
      num_vagas++;
      pthread_mutex_unlock(&lock);
      sem_post(&sem);

      sleep(8);

      sem_wait(&sem);
      pthread_mutex_lock(&lock);
      printf("func: %ld -> lavagem cliente: %d, terminada indo para o próximo\n", (long) pthread_self(), cliente);
    }
    else
    {

      printf("func: %ld -> sem clientes, total de vagas disponiveis %i \n", (long) pthread_self(), num_vagas);
      pthread_mutex_unlock(&lock);
      sem_post(&sem);

      sleep(10);

      sem_wait(&sem);
      pthread_mutex_lock(&lock);
    }
    pthread_mutex_unlock(&lock);
    sem_post(&sem);
  }
  pthread_exit(NULL);
}

void *loja(void *arg)
{
  for (int i = 0; i < num_clientes; i++)
  {
    sem_wait(&sem);
    pthread_mutex_lock(&lock);
    if (num_vagas > 0)
    {
      printf("loja: %ld -> cliente %i entrou na fila para lavagem\n", (long) pthread_self(), i);
      Enqueue(i);
      num_vagas--;
    }
    else
    {
      printf("loja: %ld -> sem vagas para o cliente: %d\n", (long) pthread_self(), i);
    }
    pthread_mutex_unlock(&lock);
    sem_post(&sem);
    sleep(2);
  }
  pthread_exit(NULL);
}