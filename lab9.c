#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <SDL3/SDL.h>
#include <graph.h>
#include <limits.h>

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

// @param fcnt the number of functions to graph
void MakeGraph(int fcnt, int (**sortFunctions)(int n, int arr[]), char **functionNames, int iterations, int step) {
    int **timeArrays = (int **) malloc(sizeof(int *) * fcnt);
    int *numberArray = (int *) malloc(sizeof(int) * iterations);
    
    if (timeArrays == NULL || numberArray == NULL) {
        perror("Memory allocation failed!");
        return;
    }
    
    // Get test data by running all the sort functions and measuring time
    int max = INT_MIN, min = INT_MAX;
    for (int i = 0; i < fcnt; i++) {
        timeArrays[i] = (int *) malloc(sizeof(int) * iterations);
        
        if (timeArrays[i] == NULL) {
            perror("Memory allocation failed!");
            return;
        }
        
        int *arr = (int *) malloc(sizeof(int) * step * iterations);

        // Fill time array for i-th sort function
        for (int j = 0; j < iterations; j++) {
            int n = step * (j + 1);
            FillRand(n, arr);
            
            timeArrays[i][j] = (sortFunctions[i])(n, arr) / 1000;
            numberArray[j] = n / 1000;
            
            if (timeArrays[i][j] > max) {
                max = timeArrays[i][j];
            }
            if (timeArrays[i][j] < min) {
                min = timeArrays[i][j];
            }
            
        }
        free(arr);
    }
    
    // Generate graph y-labels
    int lstep = (int) (SDL_pow(10, SDL_round(SDL_log10(max))) / 10);
    int lcnt = 1;
    while (lcnt * lstep < max) {
        lcnt++;
    }

    int *labelsArray = (int *) malloc(sizeof(int) * lcnt);
    for (int i = 0; i < lcnt; i++) {
        labelsArray[i] = lstep * i;
    }


    GraphInit();
    bool quit = { false };

    //The event data
    SDL_Event e;
    while( quit == false ){
                //Get event data
                while( SDL_PollEvent( &e ) )
                {
                    //If event is quit type
                    if( e.type == SDL_EVENT_QUIT )
                    {
                        //End the main loop
                        GraphQuit();
                        quit = true;
                    }
                }

                /* Draw the text */
                GraphIterate(fcnt, iterations, lcnt, timeArrays, functionNames, numberArray, labelsArray);
    }

    // Free memory
    for (int i = 0; i < fcnt; i++) {
        free(timeArrays[i]);
    }
    free(timeArrays);
    free(numberArray);
    free(labelsArray);
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

    printf("|  N  |  M+C theor.  |      HeapSort (M + C)     |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);

        if (A == NULL) {
            perror("Memory allocation failed!");
            return;
        }

        int Ttheor = CalculateSortTime(n);

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

int main() {
    printf("Heap sort tests:\n");
    RunTests(HeapSort);

    printf("Build heap time complexity:\n\n");
    PrintBuildTimeTable(BuildMaxHeap);

    printf("\nHeapSort time complexity:\n\n");
    PrintTimeTable(HeapSort);

    int (*functions[])(int, int[]) = {ShellSort, HeapSort};
    char* functionNames[] = {"ShellSort", "HeapSort"};
    int n = sizeof(functionNames) / sizeof(functionNames[0]);
    MakeGraph(n, functions, functionNames, 15, 100000);
    return 0;
}