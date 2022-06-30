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

  if (pthread_create(&thid1, NULL, funcionario, "thread 1") != 0)
    perror("pthread_create() error");

  if (pthread_create(&thid2, NULL, loja, (void *)ptr_char) != 0)
    exit(1);

  if (pthread_join(thid1, NULL) != 0)
    exit(3);

  if (pthread_join(thid2, NULL) != 0)
    exit(3);

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
      // checa mutex e aguarda
      // coloca mutex em 1
      Dequeue();
      num_vagas++;
      // coloca mutex em 0
      printf("lavagem cliente: %d, terminada indo para o próximo\n", front->data);
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
      printf("cliente %i entrou na fila para lavagem\n", i);      
      Enqueue(i);
      num_vagas--;
    }
    else
    {
      printf("sem vagas para o cliente: %d\n", i);
    }
    sleep(5);
  }
  pthread_exit(NULL);
}