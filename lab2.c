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


int bubbleSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            C++;
            // if (arr[j] < arr[i]) {
            //     int temp = arr[i];
            //     arr[i] = arr[j];
            //     arr[j] = temp;
            //     M += 3;
            // }
            if (arr[j] < arr[j - 1]) { 
                int temp = arr[j - 1];
                arr[j - 1] = arr[j];
                arr[j] = temp;
                M += 3;
            }
        }
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);

    return (M + C);
}


void printData(int n, int arr[]) {
    srand(time(NULL));

    printf("|  N  |  M+C theor.  |       Mfact + Cfact       |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int A[n];
        int C = n * (n - 1) / 2;
        int M = 3*C;
        int Ttheor = M + C;

        FillDec(n, A);
        int Tdec = bubbleSort(n, A);

        FillRand(n, A);
        int Trand = bubbleSort(n, A);

        FillInc(n, A);
        int Tinc = bubbleSort(n, A);

        printf("| %3d |  %10d  | %6d | %7d | %6d |\n", n, Ttheor, Tdec, Trand, Tinc);
    }
}

int main() { 
    int n = 10, A[10];
    
    // Incremental array checks
    FillInc(n, A);
    printf("\nIncremental array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    bubbleSort(n, A);
    printf("\nSorted incremental array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    
    // Decremental array checks
    FillDec(n, A);
    printf("\nDecremental array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    bubbleSort(n, A);
    printf("\nSorted decremental array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    
    
    // Random array checks
    FillRand(n, A);
    printf("\nRandom array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n", RunNumber(n, A), CheckSum(n, A));
    bubbleSort(n, A);
    printf("\nSorted random array:\n");
    PrintMas(n, A);
    printf("Run number: %d, Checksum: %d\n\n", RunNumber(n, A), CheckSum(n, A));

    printData(n, A);
    


    return 0;
}