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

void FillStackRand(tLE **head, int n, int max) {
  srand(time(NULL));
  for (int i = 0; i < n; i++) {
    AddToStack(head, rand() % max);
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

int DigitalSort(tLE **S, int L, int reverse) {
  // L - size of data in bytes
  // KDI (Key Digit Index) - order of bytes to sort
  int *KDI = (int *)malloc(sizeof(int) * L);
  for (int i = 0; i < L; i++) {
    // KDI[i] = L - (i + 1);
    KDI[i] = i;
  }

  int moves = 0;
  tQueue q[256];
  tLE *p, *temp;
  unsigned char d;
  for (int j = 0; j < L; j++) {
    for (int i = 0; i < 256; i++) {
      InitializeQueue(&q[i]);
    }
    p = *S;
    while (p != NULL) {
      moves++;
      d = p->data.digit[KDI[j]];
      q[d].tail->next = p;
      q[d].tail = p;
      p = p->next;
    }
    p = temp = (tLE *)S;

    if (reverse) {
      // Merge queues in descending order
      for (int i = 255; i >= 0; i--) {
        if (q[i].tail != (tLE *)&(q[i].head)) {
          moves++;
          p->next = q[i].head;
          p = q[i].tail;
        }
      }
    } else {
      // Merge queues in ascending order
      for (int i = 0; i < 256; i++) {
        if (q[i].tail != (tLE *)&(q[i].head)) {
          moves++;
          p->next = q[i].head;
          p = q[i].tail;
        }
      }
    }

    p->next = NULL;
    (*S) = temp->next;
  }

  return moves;
}

int main() {
  int n = 10;
  int m = 0;

  tLE *stack = NULL, *a = NULL, *b = NULL;

  printf(ANSI_COLOR_GREEN "Random list (size %d): \n" ANSI_COLOR_RESET, n);
  FillStackRand(&stack, n, 100);
  PrintList(stack);
  printf("Checksum: %d, series: %d\n", CheckSum(stack), RunNumber(stack));

  printf("\n");

  int Mtheor = 4 * (n + 256), Mfact = 0;
  printf(ANSI_COLOR_GREEN "Digital sort: \n" ANSI_COLOR_RESET);
  Mfact = DigitalSort(&stack, 4, 0);

  printf(ANSI_COLOR_GREEN
         "\nDigital sort complexity: " ANSI_COLOR_CYAN
         "theoretical M = %d, factual M = %d\n" ANSI_COLOR_RESET,
         Mtheor, Mfact);
  PrintList(stack);
  printf("Checksum: %d, series: %d\n", CheckSum(stack), RunNumber(stack));

  printf("\n");

  // 2-byte integer array
  stack = NULL;
  FillStackRand(&stack, n, 32767);
  printf(ANSI_COLOR_GREEN
         "Random 2-byte integer list (size %d): \n" ANSI_COLOR_RESET,
         n);
  PrintList(stack);

  DigitalSort(&stack, 2, 0);
  printf(ANSI_COLOR_GREEN
         "2-byte integer list ascending order: \n" ANSI_COLOR_RESET);
  PrintList(stack);

  DigitalSort(&stack, 2, 1);
  printf(ANSI_COLOR_GREEN
         "2-byte integer list descending order: \n" ANSI_COLOR_RESET);
  PrintList(stack);

  printf("\n");
  // 4-byte integer array
  stack = NULL;
  FillStackRand(&stack, n, 2147483647);
  printf(ANSI_COLOR_GREEN
         "Random 4-byte integer list (size %d): \n" ANSI_COLOR_RESET,
         n);
  PrintList(stack);

  DigitalSort(&stack, 4, 0);
  printf(ANSI_COLOR_GREEN
         "4-byte integer list ascending order: \n" ANSI_COLOR_RESET);
  PrintList(stack);

  DigitalSort(&stack, 4, 1);
  printf(ANSI_COLOR_GREEN
         "4-byte integer list descending order: \n" ANSI_COLOR_RESET);
  PrintList(stack);

  // Time table for DigitalSort
  printf("|   N   |        M      |              Mfact              |\n");
  printf("|       |     Theor.    |   Dec.   |   Rand.   |   Inc.   |\n");
  for (int j = 100; j <= 500; j += 100) {
    int Ttheor = 4 * (j + 256);

    ClearList(stack);
    stack = NULL;
    FillStackRand(&stack, j, 1000);
    Mfact = DigitalSort(&stack, 4, 0);
    int Trand = Mfact;

    ClearList(stack);
    stack = NULL;
    FillStackInc(&stack, j);
    Mfact = DigitalSort(&stack, 4, 0);
    int Tinc = Mfact;

    ClearList(stack);
    stack = NULL;
    FillStackDec(&stack, j);
    Mfact = DigitalSort(&stack, 4, 0);
    int Tdec = Mfact;

    printf("| %5d | %13d | %8d | %9d | %8d |\n", j, Ttheor, Tdec, Trand, Tinc);
  }
  return 0;
}