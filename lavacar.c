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

int num_vagas = 3;
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
  pthread_t thid1, thid2;

  char *ptr_char = "thread 2";

  // iniciar o semaforo deixando 2 threads acessar a região crítica
  sem_init(&sem, 0, 2);
  pthread_mutex_init(&lock, NULL);

  if (pthread_create(&thid1, NULL, funcionario, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid2, NULL, loja, (void *)ptr_char) != 0)
    exit(1);

  if (pthread_join(thid1, NULL) != 0)
    exit(3);

  if (pthread_join(thid2, NULL) != 0)
    exit(3);

  pthread_mutex_destroy(&lock);
  sem_destroy(&sem);

  pthread_exit(NULL);
}

void *funcionario(void *arg)
{
  while (1)
  {
    if (front != NULL)
    {
      printf("lavando carro do cliente: %d \n", front->data);
      sleep(8);      
      printf("lavagem cliente: %d, terminada indo para o próximo\n", front->data);

      sem_wait(&sem);
      pthread_mutex_lock(&lock);
      Dequeue();
      num_vagas++;
      pthread_mutex_unlock(&lock);
      sem_post(&sem);
    }
    else
    {
      printf("sem clientes, total de vagas disponiveis %i \n", num_vagas);
      sleep(10);
    }
  }
  pthread_exit(NULL);
}

void *loja(void *arg)
{
  for (int i = 0; i < num_clientes; i++)
  {
    if (num_vagas > 0)
    {
      sem_wait(&sem);
      pthread_mutex_lock(&lock);
      printf("cliente %i entrou na fila para lavagem\n", i);      
      Enqueue(i);
      num_vagas--;
      pthread_mutex_unlock(&lock);
      sem_post(&sem);
    }
    else
    {
      printf("sem vagas para o cliente: %d\n", i);
    }
    sleep(5);
  }
  pthread_exit(NULL);
}