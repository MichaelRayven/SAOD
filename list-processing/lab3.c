#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

typedef struct tLE {
  struct tLE *next;
  union {
    int value;
    unsigned char digit[sizeof(int)];
  } data;
} tLE;

typedef struct tQueue {
  struct tLE *head;
  struct tLE *tail;
} tQueue;

// Stack operations
void AddToStack(tLE **head, int data) {
  tLE *p = (tLE *)malloc(sizeof(tLE));
  p->data.value = data;
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
void InitializeQueue(tQueue *q) {
  q->head = NULL;
  q->tail = (tLE *)&q->head;
}

void AddToQueue(tQueue *queue, int data) {
  tLE *p = (tLE *)malloc(sizeof(tLE));
  p->data.value = data;
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
    printf("%d ", p->data.value);
    p = p->next;
  }

  printf("\n");
}

int CheckSum(tLE *head) {
  int sum = 0;
  tLE *p = head;

  while (p != NULL) {
    sum += p->data.value;
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
    if (p->data.value > p->next->data.value) {
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

  printf("%d ", head->data.value);
  PrintListRecursive(head->next);
}

void PrintListBackwardsRecursive(tLE *head) {
  if (head == NULL) {
    return;
  }
  PrintListBackwardsRecursive(head->next);
  printf("%d ", head->data.value);
}

// L - size of data in bytes
void DigitalSort(tLE *S, int L) {
  tQueue queues[256];
  tLE *p;
  for (int j = 0; j < L; j++) {
    for (int i = 0; i < 256; i++) {
      InitializeQueue(&queues[i]);
    }
    p = S;
    while (p != NULL) {
      unsigned char d = p->data.digit[j];
      queues[d].tail->next = p;
      queues[d].tail = p;
      p = p->next;
    }
    p = S;
    for (int i = 0; i < 256; i++) {
      if (queues[i].tail != queues[i].head) {
        p->next = queues[i].head;
        p = queues[i].tail;
      }
    }
    p->next = NULL;
  }
}

int main() {
  int n = 32;
  int m = 0;

  tLE *stack = NULL, *a = NULL, *b = NULL;

  printf(ANSI_COLOR_GREEN "Random list (size %d): \n" ANSI_COLOR_RESET, n);
  FillStackRand(&stack, n);
  PrintList(stack);
  printf("Checksum: %d, series: %d\n", CheckSum(stack), RunNumber(stack));

  printf("\n");

  return 0;
}