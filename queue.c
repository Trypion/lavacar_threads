#include <stdio.h>
#include <stdlib.h>

struct Node
{
  int data;
  struct Node *next;
};

typedef struct Node node;
int size = 0;

node *front = NULL;
node *rear = NULL;

// adds a new node at the end of the list
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

// when calling this method before show(), the loop issue occurs
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

// if just calling Enqueue() and Show() methods, it runs normally
void Show()
{

  node *tmp = front;

  while (tmp != NULL)
  {

    printf("%d \n", tmp->data);
    tmp = tmp->next;
  }
}

int main(void)
{

  Enqueue(1);
  Enqueue(2);
  Enqueue(3);
  Enqueue(4);
  Enqueue(5);
  Enqueue(6);
  Enqueue(7);

  Dequeue();
  Dequeue();
  Dequeue();
  Dequeue();
  Show();

  return 0;
}