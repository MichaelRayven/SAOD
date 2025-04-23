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
void InitializeQueue(tQueue *q) {
  q->head = NULL;
  q->tail = (tLE *)&q->head;
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

void MergeSeries(tLE **a, int q, tLE **b, int r, tQueue *c, int *cmp,
                 int *move) {
  while (q > 0 && r > 0) {
    (*cmp)++;
    if ((*a)->data <= (*b)->data) {
      c->tail->next = *a;
      *a = (*a)->next;
      q--;
    } else {
      c->tail->next = *b;
      *b = (*b)->next;
      r--;
    }

    c->tail = c->tail->next;
    (*move)++;
  }

  while (q > 0) {
    c->tail->next = *a;
    c->tail = *a;
    *a = (*a)->next;
    q--;
    (*move)++;
  }

  while (r > 0) {
    c->tail->next = *b;
    c->tail = *b;
    *b = (*b)->next;
    r--;
    (*move)++;
  }

  c->tail->next = NULL;
}

void SplitLists(tLE *head, tLE **a, tLE **b, int *n, int *move) {
  (*a) = head;
  (*b) = head->next;

  tLE *k = (*a), *p = (*b);
  (*move)++;
  (*n) = 1; // Количество элементов

  while (p != NULL) {
    (*n)++;
    k->next = p->next;
    k = p;
    p = p->next;
    (*move)++;
  }
}

void MergeSort(tLE **S, int *comparisons, int *moves) {
  // Empty or 1 element list
  if (*S == NULL || (*S)->next == NULL)
    return;

  tQueue c[2];
  tLE *a, *b;
  int n;
  SplitLists(*S, &a, &b, &n, moves);

  for (int p = 1; p < n; p *= 2) {
    InitializeQueue(&c[0]);
    InitializeQueue(&c[1]);
    int i = 0, m = n;

    while (m > 0) {
      // Get factual a series sizes
      int q = (m >= p) ? p : m;
      m -= q;

      // Get factual b series sizes
      int r = (m >= p) ? p : m;
      m -= r;

      MergeSeries(&a, q, &b, r, &c[i], comparisons, moves);

      i = 1 - i;
    }

    a = c[0].head;
    b = c[1].head;
  }

  c[0].tail->next = NULL;
  *S = c[0].head;
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

  // Split lists tests
  int moves = 0;
  SplitLists(stack, &a, &b, &m, &moves);
  printf(ANSI_COLOR_GREEN "Split lists: \n" ANSI_COLOR_RESET);
  printf("List A: ");
  PrintList(a);
  printf("Checksum: %d, series: %d\n", CheckSum(a), RunNumber(a));
  printf("List B: ");
  PrintList(b);
  printf("Checksum: %d, series: %d\n", CheckSum(b), RunNumber(b));

  printf("\n");

  // Merge series tests
  printf(ANSI_COLOR_GREEN "Merge lists: \n" ANSI_COLOR_RESET);
  int i = 0;
  tQueue c[2];
  InitializeQueue(&c[0]);
  InitializeQueue(&c[1]);

  int Cfact = 0, Mfact = 0;
  int Ctheor = 0, Mtheor = 0;
  while (m > 0) {
    int q = (m >= 1) ? 1 : m;
    m -= q;

    int r = (m >= 1) ? 1 : m;
    m -= r;

    // min(q, r) <= C <= q + r - 1
    // This is the average
    Ctheor += ((q < r ? q : r) + q + r - 1) / 2;
    Mtheor += q + r;
    MergeSeries(&a, q, &b, r, &c[i], &Cfact, &Mfact);
    i = 1 - i;
  }

  printf("Queue C[0]: ");
  PrintList(c[0].head);
  printf("Checksum: %d, series: %d\n", CheckSum(c[0].head),
         RunNumber(c[0].head));
  printf("Queue C[1]: ");
  PrintList(c[1].head);
  printf("Checksum: %d, series: %d\n", CheckSum(c[1].head),
         RunNumber(c[1].head));

  printf(
      ANSI_COLOR_GREEN
      "\nMerge lists complexity: " ANSI_COLOR_CYAN
      "theoretical C = %d, M = %d, factual C = %d, M = %d\n" ANSI_COLOR_RESET,
      Ctheor, Mtheor, Cfact, Mfact);

  printf("\n");

  // Clean up
  ClearList(stack);
  stack = NULL;

  // Merge sort tests
  printf(ANSI_COLOR_GREEN "Merge sort: \n" ANSI_COLOR_RESET);

  printf("Random list (size %d): \n", n);
  FillStackRand(&stack, n);
  PrintList(stack);
  printf("Checksum: %d, series: %d\n", CheckSum(stack), RunNumber(stack));

  printf("\n");
  
  Cfact = 0;
  Mfact = 0;
  MergeSort(&stack, &Cfact, &Mfact);
  printf("Sorted random list: \n");
  PrintList(stack);
  printf("Checksum: %d, series: %d\n", CheckSum(stack), RunNumber(stack));
  
  Ctheor = log2(n) * n;
  Mtheor = log2(n) * n + n;
  printf(
    ANSI_COLOR_GREEN
    "\nMerge sort complexity: " ANSI_COLOR_CYAN
    "theoretical C = %d, M = %d, factual C = %d, M = %d\n" ANSI_COLOR_RESET,
    Ctheor, Mtheor, Cfact, Mfact);
    
  printf("\n");
  
  // Time table for MergeSort
  printf("|   N   |     M + C     |          Mfact + Cfact          |\n");
  printf("|       |     Theor.    |   Dec.   |   Rand.   |   Inc.   |\n");
  for (int j = 100; j < 500; j += 100) {

    int Ttheor = log2(j) * j + log2(j) * j + j;

    ClearList(stack);
    stack = NULL;
    Cfact = 0;
    Mfact = 0;
    FillStackRand(&stack, j);
    MergeSort(&stack, &Cfact, &Mfact);
    int Trand = Cfact + Mfact;
    
    ClearList(stack);
    stack = NULL;
    Cfact = 0;
    Mfact = 0;
    FillStackInc(&stack, j);
    MergeSort(&stack, &Cfact, &Mfact);
    int Tinc = Cfact + Mfact;

    ClearList(stack);
    stack = NULL;
    Cfact = 0;
    Mfact = 0;
    FillStackDec(&stack, j);
    MergeSort(&stack, &Cfact, &Mfact);
    int Tdec = Cfact + Mfact;

    printf("| %5d | %13d | %8d | %9d | %8d |\n", j, Ttheor, Tdec, Trand, Tinc);
  }

  return 0;
}