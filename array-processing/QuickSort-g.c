#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <limits.h>

typedef struct {
    int depth;
    int C;
    int M;
} Data;

int readInt(int *number) {
    char line[64];
    fgets(line, sizeof(line), stdin);

    if (sscanf(line, "%d", number) != 1) {
        return 0;
    } else {
        return 1;
    }
}

void FillInc(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        A[i] = i + 1;
    }
}

void FillDec(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        A[i] = n - i;
    }
}

void FillRand(int n, int A[]) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        A[i] = rand() % 100;
    }
}

int CheckSum(int n, int A[]) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += A[i];
    }
    return sum;
}

int RunNumber(int n, int A[]) {
    if (n == 0) return 0;
    int series = 1;
    for (int i = 1; i < n; i++) {
        if (A[i] < A[i - 1]) {
            series++;
        }
    }
    return series;
}

void PrintMas(int n, int A[]) {
    for (int i = 0; i < n; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

/*
Copies all elements from source array into the target array.
@param n length of array A and B.
@param A source array.
@param B target array.
*/
void CopyMas(int n, int A[], int B[]) {
    for (int i = 0; i < n; i++) {
        B[i] = A[i];
    }
}

int CalculateBuildTime(int n) {
    int C = 2 * log2(n);
    int M = log2(n) + 2;
    return M + C;
}

int CalculateSortTime(int n) {
    int C = 2 * n * log2(n) + n + 2;
    int M = n * log2(n) + 6.5 * n - 4;
    return M + C;
}

void PrintBuildTimeTable(int (*sortFunction)(int arr[], int L, int R)) {
    srand(time(NULL));

    printf("|  N  |  M+C theor.  |       Mfact + Cfact       |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);

        if (A == NULL) {
            perror("Memory allocation failed!");
            return;
        }
        int Ttheor = CalculateBuildTime(n - 1);

        FillDec(n, A);
        int Tdec = sortFunction(A, 0, n - 1);

        FillRand(n, A);
        int Trand = sortFunction(A, 0, n - 1);

        FillInc(n, A);
        int Tinc = sortFunction(A, 0, n - 1);

        printf("| %3d |  %10d  | %6d | %7d | %6d |\n", n, Ttheor, Tdec, Trand, Tinc);

        free(A);
    }
}

void PrintTimeTable(int (*sortFunction)(int n, int arr[])) {
    srand(time(NULL));

    printf("|  N  |  M+C theor.  |      QuickSort (M + C)    |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);

        if (A == NULL) {
            perror("Memory allocation failed!");
            return;
        }
        int M = 3 * (n - 1), C= (pow(n, 2) + 5 * n + 4) / 2;
        int Ttheor = M + C;

        FillDec(n, A);
        int Tdec = sortFunction(n, A);

        FillRand(n, A);
        int Trand = sortFunction(n, A);

        FillInc(n, A);
        int Tinc = sortFunction(n, A);

        printf("| %3d |  %10d  | %6d | %7d | %6d |\n", n, Ttheor, Tdec, Trand, Tinc);

        free(A);
    }
}

void RunTests(int (*sortFunction)(int n, int arr[])) {
    int arr[10];
    int n = 10;

    // Incremental array checks
    FillInc(n, arr);
    printf("\nIncremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted incremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));

    // Decremental array checks
    FillDec(n, arr);
    printf("\nDecremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted decremental array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));


    // Random array checks
    FillRand(n, arr);
    printf("\nRandom array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, arr), CheckSum(n, arr));
    sortFunction(n, arr);
    printf("\nSorted random array:\n");
    PrintMas(n, arr);
    printf("Run number: %d, Checksum: %d\n\n", RunNumber(n, arr), CheckSum(n, arr));
}

int* GenerateSteps(int n, int *m) {
    int h = 1, i = 0;
    int size = n/2;
    int *arr = malloc(sizeof(int) * size);
    double gamma = 2.243609061420001;
    
    
    while (h < n) {
        if (i >= size) {
            size += 10;
            arr = realloc(arr, sizeof(int) * size);
        }

        arr[i++] = h;
        h = (int) ((pow(gamma, (i + 1)) - 1)/(gamma - 1)) + 1;
    }
    
    *m = i;

    return arr;
}

int ShellSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения
    int m = 0; // Количество k-сортировок
    int *H = GenerateSteps(n, &m);

    while (m > 0) {
        int k = H[--m]; // Шаг
        
        for (int i = k; i < n; i++) {
            int temp = arr[i];
            int j = i - k;
            M++;
            
            C++;
            for (; j >= 0 && temp < arr[j]; j -= k) {
                arr[j + k] = arr[j];
                C++;
                M++;
            }
            
            arr[j + k] = temp;
            M++;
        }
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);
    free(H);
    return M + C;
}

/*
@brief Constructs (L, R) max-heap from (L + 1, R) max-heap 
@param L index of the leftmost element
@param R index of the rightmost element
@return Time complexity
*/
int BuildMaxHeap(int A[], int L, int R) {
    int x = A[L], i = L, j = 2*i + 1;
    int M = 1, C = 0;

    while (j <= R) {
        C++;
        if (j < R && A[j + 1] >= A[j]) {
            j = j + 1;
        }

        C++;
        if (x >= A[j]) break;

        A[i] = A[j];
        M++;
        i = j;
        j = 2*i + 1;
    }

    A[i] = x;
    M++;

    return M+C;
}

int HeapSort(int n, int A[]) {
    int M = 0, buildT = 0;

    for (int L = n / 2 - 1; L >= 0; L--) {
        buildT += BuildMaxHeap(A, L, n - 1);
    }
    
    for (int R = n - 1; R > 0; R--) {
        int temp = A[0];
        A[0] = A[R];
        A[R] = temp;
        M += 3;

        buildT += BuildMaxHeap(A, 0, R - 1);
    }
    return M + buildT;
}

Data QuickSortV1(int A[], int L, int R) {
    int x = A[L], i = L, j = R;
    Data data = {0, 0, 0};
    data.M = 1;
    data.depth = 1;

    while (i <= j) {
        data.C++;
        while (A[i] < x) {
            data.C++;
            i++;
        }
        data.C++;
        while (A[j] > x) {
            data.C++;
            j--;
        }

        if (i <= j) {
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
            data.C += 3;

            i++;
            j--;
        }
    }

    if (L < j) {
        Data d = QuickSortV1(A, L, j);
        data.depth = d.depth + 1;
        data.C += d.C;
        data.M += d.M;
    }
    if (i < R) {
        Data d = QuickSortV1(A, i, R);
        data.depth = d.depth + 1;
        data.C += d.C;
        data.M += d.M;
    }

    return data;
}

Data QuickSortV2(int A[], int L, int R) {
    Data data = {0, 0, 0};
    data.depth = 1;

    while(L<R) {
        int x = A[L], i = L, j = R;
        data.M += 1;
    while (i <= j) {
        data.C++;
        while (A[i] < x) {
            data.C++;
            i++;
        }
        data.C++;
        while (A[j] > x) {
            data.C++;
            j--;
        }

        if (i <= j) {
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
            data.C += 3;

            i++;
            j--;
        }
    }

    if (j - L < R - j) {
        Data d = QuickSortV2(A, L, j);
        data.depth = d.depth + 1;
        data.C += d.C;
        data.M += d.M;
        L = i;
    } else {
        Data d = QuickSortV2(A, i, R);
        data.depth = d.depth + 1;
        data.C += d.C;
        data.M += d.M;
        R = j;
    }
}

    return data;
}

int QuickSortV1Wrapper(int n, int A[]) {
    Data data = QuickSortV1(A, 0, n - 1);
    return data.C + data.M;
}

int QuickSortV2Wrapper(int n, int A[]) {
    Data data = QuickSortV2(A, 0, n - 1);
    return data.C + data.M;
}


int main() {
    printf("Heap sort tests:\n");
    RunTests(QuickSortV1Wrapper);

    printf("\nQuickSort time complexity:\n\n");
    PrintTimeTable(QuickSortV1Wrapper);

    // Compare quick sorts with different pivots
    printf("\nCompare QuickSort1 with QuickSort2:\n\n");
    srand(time(NULL));

    printf("|  N  |      QuickSort 1 (M + C)    |      QuickSort 2 (M + C)    |\n");
    printf("|     |  Dec.  |   Rand.   |  Inc.  |  Dec.  |   Rand.   |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n), *B = (int *) malloc(sizeof(int) * n);

        if (A == NULL || B == NULL) {
            perror("Memory allocation failed!");
            return 1;
        }

        FillDec(n, A);
        CopyMas(n, A, B);
        int Tdec1 = QuickSortV1(A, 0, n - 1).depth;
        int Tdec2 = QuickSortV2(A, 0, n - 1).depth;
        
        FillRand(n, A);
        CopyMas(n, A, B);
        int Trand1 = QuickSortV1(A, 0, n - 1).depth;
        int Trand2 = QuickSortV2(A, 0, n - 1).depth;
        
        FillInc(n, A);
        CopyMas(n, A, B);
        int Tinc1 = QuickSortV1(A, 0, n - 1).depth;
        int Tinc2 = QuickSortV2(A, 0, n - 1).depth;

        printf("| %3d | %6d | %9d | %6d | %6d | %9d | %6d |\n", n, Tdec1, Trand1, Tinc1, Tdec2, Trand2, Tinc2);

        free(A);
        free(B);
    }

    return 0;
}