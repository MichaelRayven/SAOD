#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tLE {
  struct tLE *next;
  int data;
} tLE;

typedef struct tQueue {
  struct tLE *head;
  struct tLE *tail;
} tQueue;

// Stack operations
void AddToStack(tLE **head, int data) {
  tLE *p = (tLE *)malloc(sizeof(tLE));
  p->data = data;
  p->next = *head;
  *head = p;
}

void FillStackInc(tLE **head, int n) {
  for (int i = 1; i <= n; i++) {
    AddToStack(head, i);
  }
}

void FillStackDec(tLE **head, int n) {
  for (int i = n; i > 0; i--) {
    AddToStack(head, i);
  }
}

void FillStackRand(tLE **head, int n) {
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    AddToStack(head, rand() % 100);
  }
}

// Queue operations
tQueue *InitializeQueue() {
  tQueue *queue = (tQueue *)malloc(sizeof(tQueue));
  queue->head = NULL;
  queue->tail = (tLE *)&queue->head;
  return queue;
}

void AddToQueue(tQueue *queue, int data) {
  tLE *p = (tLE *)malloc(sizeof(tLE));
  p->data = data;
  p->next = NULL;

  queue->tail->next = p;
  queue->tail = p;
}

void FillQueueInc(tQueue *queue, int n) {
  for (int i = 1; i <= n; i++) {
    AddToQueue(queue, i);
  }
}

void FillQueueDec(tQueue *queue, int n) {
  for (int i = n; i > 0; i--) {
    AddToQueue(queue, i);
  }
}

void FillQueueRand(tQueue *queue, int n) {
  srand(time(NULL));
  for (int i = 1; i <= n; i++) {
    AddToQueue(queue, rand() % 100);
  }
}

// List operations
void PrintList(tLE *head) {
  tLE *p = head;

  while (p != NULL) {
    printf("%d ", p->data);
    p = p->next;
  }

  printf("\n");
}

int CheckSum(tLE *head) {
  int sum = 0;
  tLE *p = head;

  while (p != NULL) {
    sum += p->data;
    p = p->next;
  }

  return sum;
}

int RunNumber(tLE *head) {
  if (head == NULL)
    return 0;

  int series = 1;
  tLE *p = head;

  while (p->next != NULL) {
    if (p->data > p->next->data) {
        series++;
    }
    p = p->next;
  }

  return series;
}

void ClearList(tLE *head) {
  tLE *p = head;

  while (p != NULL) {
    tLE *tmp = p;
    p = p->next;
    free(tmp);
  }
}

void PrintListRecursive(tLE *head) {
  if (head == NULL) {
    printf("\n");
    return;
  }

  printf("%d ", head->data);
  PrintListRecursive(head->next);
}

void PrintListBackwardsRecursive(tLE *head) {
  if (head == NULL) {
    return;
  }
  PrintListBackwardsRecursive(head->next);
  printf("%d ", head->data);
}

int main() {
  int n = 10;

  // Stack increasing fill
  tLE *head = NULL;
  FillStackInc(&head, n);
  printf("Stack (Inc): ");
  PrintList(head);
  printf("Checksum: %d\n", CheckSum(head));
  printf("Series count: %d\n\n", RunNumber(head));
  ClearList(head);

  // Stack decreasing fill
  head = NULL;
  FillStackDec(&head, n);
  printf("Stack (Dec): ");
  PrintList(head);
  printf("Checksum: %d\n", CheckSum(head));
  printf("Series count: %d\n\n", RunNumber(head));
  ClearList(head);


  // Stack random fill
  head = NULL;
  FillStackRand(&head, n);
  printf("Stack (Random): ");
  PrintList(head);
  printf("Checksum: %d\n", CheckSum(head));
  printf("Series count: %d\n\n", RunNumber(head));
  ClearList(head);

  // Queue increasing fill
  tQueue *queue = InitializeQueue();
  FillQueueInc(queue, n);
  printf("Queue (Inc): ");
  PrintList(queue->head);
  printf("Checksum: %d\n", CheckSum(queue->head));
  printf("Series count: %d\n\n", RunNumber(queue->head));
  ClearList(queue->head);
  free(queue);

  // Queue decreasing fill
  queue = InitializeQueue();
  FillQueueDec(queue, n);
  printf("Queue (Dec): ");
  PrintList(queue->head);
  printf("Checksum: %d\n", CheckSum(queue->head));
  printf("Series count: %d\n\n", RunNumber(queue->head));
  ClearList(queue->head);
  free(queue);

  // Queue decreasing fill
  queue = InitializeQueue();
  FillQueueRand(queue, n);
  printf("Queue (Random): ");
  PrintList(queue->head);
  printf("Checksum: %d\n", CheckSum(queue->head));
  printf("Series count: %d\n\n", RunNumber(queue->head));
  ClearList(queue->head);
  free(queue);

  // Тестирование очереди и рекурсии со случайными числами
  queue = InitializeQueue();
  FillQueueRand(queue, n);

  printf("Queue in regular order: ");
  PrintListRecursive(queue->head);

  printf("Queue in reversed order: ");
  PrintListBackwardsRecursive(queue->head);
  printf("\n");

  ClearList(queue->head);
  free(queue);

  return 0;
}