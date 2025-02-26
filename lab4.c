#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <SDL3/SDL.h>
#include <graph.h>

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

void PrintTimeTable(int (*sortFunction)(int n, int arr[])) {
    srand(time(NULL));

    printf("|  N  |  M+C theor.  |       Mfact + Cfact       |\n");
    printf("|     |              |  Dec.  |  Rand.  |  Inc.  |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);

        if (A == NULL) {
            perror("Memory allocation failed!");
            return;
        }

        int C = n * (n - 1) / 2;
        int M = C + n*2 - 2;
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

void PrintComparisonTable(int (*sortFunctionA)(int n, int arr[]), int (*sortFunctionB)(int n, int arr[]), int (*sortFunctionC)(int n, int arr[]), int (*sortFunctionD)(int n, int arr[])) {
    srand(time(NULL));

    printf("|  N  |                                     Mfact + Cfact                                 |\n");
    printf("|     |     SelectSort     |     BubbleSort     |     ShakerSort     |     InsertSort     |\n");

    for (int n = 100; n <= 500; n += 100) {
        int *A = (int *) malloc(sizeof(int) * n);
        int *B = (int *) malloc(sizeof(int) * n);
        FillRand(n, A);

        if (A == NULL || B == NULL) {
            perror("Memory allocation failed!");
            return;
        }
        
        CopyMas(n, A, B);
        int TrandA = sortFunctionA(n, B);

        CopyMas(n, A, B);
        int TrandB = sortFunctionB(n, B);

        CopyMas(n, A, B);
        int TrandC = sortFunctionC(n, B);

        CopyMas(n, A, B);
        int TrandD = sortFunctionD(n, B);

        printf("| %3d | %18d | %18d | %18d | %18d |\n", 
            n, TrandA, TrandB, TrandC, TrandD);

        // Clear memory
        free(A);
        free(B);
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

// fcnt - the number of functions to graph
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
        
        int n = step;
        // Fill time array for i-th sort function
        for (int j = 0; j < iterations; j++) {
            int *arr = (int *) malloc(sizeof(int) * n);
            FillRand(n, arr);
            
            timeArrays[i][j] = (sortFunctions[i])(n, arr);
            numberArray[j] = n;
            
            if (timeArrays[i][j] > max) {
                max = timeArrays[i][j];
            }
            if (timeArrays[i][j] < min) {
                min = timeArrays[i][j];
            }
            
            free(arr);
            n += step;
        }
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

int shakerSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения
    int L = 0, R = n - 1, k = n - 1;

    do {
        for (int i = R; i > L; i--) {
            C++;
            if (arr[i] < arr[i - 1]) {
                int temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                M += 3;
                k = i;
            }
        }
        L = k;

        for (int i = L; i < R; i++) {
            C++;
            if (arr[i] > arr[i + 1]) {
                int temp = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = temp;
                M += 3;
                k = i;
            }
        }
        R = k;

    } while(L < R);

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);

    return (M + C);
}

int bubbleSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 0; i < n - 1; i++) {
        for (int j = n - 1; j > i; j--) {
            C++;
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

int insertSort(int n, int arr[]) {
    int C = 0, M = 0; // Сравнения и перемещения

    for (int i = 1; i < n; i++) {
        int temp = arr[i];
        int j = i - 1;
        M++;
        
        for (;j >= 0 && temp < arr[j]; C++) {
            arr[j + 1] = arr[j];
            M++;
            j--;
        }
        
        arr[j + 1] = temp;
        M++;
    }

    // printf("\n* Comparisons: %d, moves: %d *\n", C, M);
    return M + C;
}

int main() {  
    srand(time(NULL));
    
    RunTests(insertSort);
    printf("Shaker sort:\n");
    PrintTimeTable(insertSort);
    printf("\nComparison between sort methods:\n");
    PrintComparisonTable(selectSort, bubbleSort, shakerSort, insertSort);
    
    int (*functions[])(int, int[]) = {selectSort, shakerSort, bubbleSort, insertSort};
    char* functionNames[] = {"SelectSort", "ShakerSort", "BubbleSort", "InsertSort"};
    int n = sizeof(functionNames) / sizeof(functionNames[0]);
    MakeGraph(n, functions, functionNames, 10, 100);

    return 0;
}