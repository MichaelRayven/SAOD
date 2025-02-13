#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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


int selectSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[minInd]) {
                minInd = j;
                C++;
            }
        }
        
        int temp = arr[i];
        arr[i] = arr[minInd];
        arr[minInd] = temp;
        M += 3;
    }

    return (M + C);
}

int improvedSelectSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        int minInd = i;
        for (int j = i + 1; j < n; j++) {
            C++;
            if (arr[j] < arr[minInd]) {
                minInd = j;
            }
        }
        C++;
        if (arr[j] != arr[minInd]) {
            int temp = arr[i];
            arr[i] = arr[minInd];
            arr[minInd] = temp;
            M += 3;
        }
    }

    return (M + C);
}

void printData(int n, int arr[]) {
    int C = n * (n - 1) / 2;
    int M = 3 * (n - 1);

    printf("N | M+C theor.\n");
    printf("%d | %d \n", n, M + C);
    
    printf("Initial Mf + Cf \n");
    printf("Dec. | Rand. | Inc. \n");

    FillDec(n, arr);
    printf("%d |", selectSort(n, arr));
    FillRand(n, arr);
    printf("%d |", selectSort(n, arr));
    FillInc(n, arr);
    printf("%d\n", selectSort(n, arr));

    printf("Improved Mf + Cf \n");
    printf("Dec. | Rand. | Inc. \n");

    FillDec(n, arr);
    printf("%d |", improvedSelectSort(n, arr));
    FillRand(n, arr);
    printf("%d |", improvedSelectSort(n, arr));
    FillInc(n, arr);
    printf("%d\n", improvedSelectSort(n, arr));
}

int main() {
    int n = 10;
    int A[n];
    
    printData(n, A);


 
    // // Incremental array checks
    // FillInc(n, A);
    // printf("\nIncremental array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    // selectSort(n, A);
    // printf("\nSorted incremental array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    
    // // Decremental array checks
    // FillDec(n, A);
    // printf("\nDecremental array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    // selectSort(n, A);
    // printf("\nSorted decremental array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    
    
    // // Random array checks
    // FillRand(n, A);
    // printf("\nRandom array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    // selectSort(n, A);
    // printf("\nSorted random array:\n");
    // PrintMas(n, A);
    // printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    
    return 0;
}